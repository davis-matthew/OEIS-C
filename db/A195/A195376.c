/*
 * A195376  Parity of EKG sequence A064413.
 * https://oeis.org/A195376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064413(n) mod 2 (parity of the EKG sequence)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  static state -------------------------------------------------------- */
static idx_t max_n = 0;          /* highest index computed */
static seq_t *parity = NULL;     /* 1‑based parity array */
static idx_t parity_cap = 0;     /* capacity of parity array (max index) */
static bool *used = NULL;        /* 1‑based used flag array */
static idx_t used_cap = 0;       /* capacity of used array (max value + 1) */
static uint64_t last_val = 0;    /* current last value of the EKG sequence */

/* --------------------------------------------------------------------- */
/*  helpers -------------------------------------------------------------- */
static void ensure_parity_cap(idx_t n)
{
    if (n > parity_cap) {
        idx_t new_cap = (parity_cap << 1) ? parity_cap << 1 : 64;
        if (new_cap < n) new_cap = n;
        seq_t *new_arr = (seq_t *)realloc(parity, (new_cap + 1) * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        memset(new_arr + parity_cap + 1, 0, (new_cap - parity_cap) * sizeof(seq_t));
        parity = new_arr;
        parity_cap = new_cap;
    }
}

static void ensure_used_cap(idx_t v)
{
    if (v >= used_cap) {
        idx_t new_cap = (used_cap << 1) ? used_cap << 1 : 64;
        if (new_cap <= v) new_cap = v + 1;
        bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!new_used) exit(EXIT_FAILURE);
        memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = new_used;
        used_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  sequence ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* first‑time initialization */
    if (parity == NULL) {
        parity_cap = 2;
        parity     = (seq_t *)calloc(parity_cap + 1, sizeof(seq_t));
        if (!parity) exit(EXIT_FAILURE);
        parity[1] = 1;                       /* A064413(1) = 1 (odd)   */
        parity[2] = 0;                       /* A064413(2) = 2 (even)  */
        used_cap = 64;
        used     = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) exit(EXIT_FAILURE);
        used[1] = true;
        used[2] = true;
        max_n = 2;
        last_val = 2;
    }

    /* generate terms up to the requested index */
    while (max_n < n) {
        uint64_t v = last_val;
        uint64_t m = v;
        uint64_t primes[64];
        size_t pcnt = 0;

        /* factorise v into distinct primes */
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                primes[pcnt++] = p;
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) primes[pcnt++] = m;

        /* find smallest unused multiple of any prime factor */
        uint64_t min_candidate = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            uint64_t cand = p;
            for (;;) {
                idx_t ci = (idx_t)cand;
                ensure_used_cap(ci);
                if (!used[ci]) break;
                cand += p;
            }
            if (min_candidate == 0 || cand < min_candidate)
                min_candidate = cand;
        }

        /* mark the chosen number as used */
        idx_t idx_cand = (idx_t)min_candidate;
        ensure_used_cap(idx_cand);
        used[idx_cand] = true;

        /* store parity of the new term */
        idx_t next_idx = max_n + 1;
        ensure_parity_cap(next_idx);
        parity[next_idx] = (seq_t)(min_candidate & 1ULL);

        /* advance state */
        last_val = min_candidate;
        max_n = next_idx;
    }

    return parity[n];
}
