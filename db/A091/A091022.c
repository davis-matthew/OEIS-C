/*
 * A091022  Semiprimes with semiprime indices.
 * https://oeis.org/A091022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001358(A001358(n)) The first part of the computation
 *            obtains the n‑th semiprime, the second part reads the semiprime
 *            whose index is that value
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build a list of the first MAX_SEM COUNT semiprimes by sieving the
   smallest prime factor for each number up to LIMIT. This is a one‑time
   cost that allows O(1) look‑ups of both semiprime values and indices. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *semiprimes = NULL;   /* 1‑based array of semiprime values */
static idx_t   semiprime_cnt = 0;  /* number of semiprimes stored */

/* How many semiprimes are enough for n ≤ 10000?  The 10 000‑th semiprime
   is about 45 000, so the index we ever need as a second level is ≤ 45 000.
   We generate the first 60 000 semiprimes to be safe. */
#define MAX_SEM_CNT 60000
/* Upper bound for the value of the largest semiprime we have to generate.
   The 60 000‑th semiprime is around 250 000, so we take 400 000 for safety. */
#define LIMIT 400000

/* ============================================================
   Helper: generate the list of the first MAX_SEM_CNT semiprimes.
   ============================================================ */
static void init_semiprimes(void)
{
    uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return;            /* allocation failed – give up */

    /* smallest prime factor sieve */
    for (uint32_t i = 2; i <= LIMIT; ++i) spf[i] = i;
    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    semiprimes = (seq_t *)malloc((MAX_SEM_CNT + 1) * sizeof(seq_t));
    if (!semiprimes) { free(spf); return; }

    semiprime_cnt = 0;
    for (uint32_t n = 4; n <= LIMIT && semiprime_cnt < MAX_SEM_CNT; ++n) {
        uint32_t x = n;
        int pfc = 0;                 /* prime‑factor count (with multiplicity) */
        while (x > 1 && pfc <= 2) {
            ++pfc;
            x /= spf[x];
        }
        if (pfc == 2) {
            ++semiprime_cnt;
            semiprimes[semiprime_cnt] = n;
        }
    }

    free(spf);    /* sieve no longer needed */
}

/* ============================================================
   The public function implementing the sequence.
   ============================================================ */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_semiprimes();
        initialized = 1;
    }
    /* Guard against out of bounds, though n is expected ≥ 1 and ≤ 10000. */
    if (n < 1 || (idx_t)n > semiprime_cnt) return 0;

    idx_t idx = (idx_t)semiprimes[(size_t)n];
    if (idx < 1 || idx > semiprime_cnt) return 0;
    return semiprimes[(size_t)idx];
}
