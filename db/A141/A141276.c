/*
 * A141276  a(n) = A338038(A002808(n)).
 * https://oeis.org/A141276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A338038(A002808(n))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- internal state ----------------------------------------- */
static seq_t *composites = NULL;        /* array of composite numbers   */
static size_t composites_cap = 0;       /* capacity of the above array  */
static size_t composites_cnt = 0;       /* how many composites stored   */

static uint32_t *spf = NULL;            /* smallest prime factor array  */
static size_t spf_cap = 0;              /* capacity of spf array        */

static idx_t last_limit = 0;            /* last sieved limit            */

/* ---------- helper to ensure we have at least n composites ---------- */
static void ensure_composites(idx_t n)
{
    while (composites_cnt < (size_t)n) {
        /* increase sieve limit */
        idx_t new_limit = (last_limit == 0) ? 50 : last_limit * 2;

        /* reallocate spf array */
        uint32_t *new_spf = (uint32_t *)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
        if (!new_spf) abort();
        spf = new_spf;
        spf_cap = new_limit;

        /* initialise spf with zeros */
        for (idx_t i = 0; i <= new_limit; ++i) spf[i] = 0;

        /* simple sieve of Eratosthenes with spf */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (spf[i] == 0) {             /* i is prime */
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * i <= (uint64_t)new_limit) {
                    for (idx_t j = i * i; j <= new_limit; j += i) {
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* collect composites > last_limit up to new_limit */
        for (idx_t num = last_limit + 1; num <= new_limit; ++num) {
            if (num >= 2 && spf[num] != (uint32_t)num) { /* composite */
                if (composites_cap <= composites_cnt) {
                    composites_cap = composites_cap ? composites_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(composites, composites_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    composites = tmp;
                }
                composites[composites_cnt++] = (seq_t)num;
            }
        }
        last_limit = new_limit;
    }
}

/* ---------- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) corresponds to the first composite (4) etc. */
    if (n < 1) return 0;         /* out of range, not expected */

    /* make sure we have at least n composites available */
    ensure_composites(n);

    /* fetch the nth composite number (1‑based) */
    seq_t comp = composites[n - 1];

    /* factorise comp using spf and compute A338038 value */
    idx_t m = (idx_t)comp;
    seq_t sum = 0;

    while (m > 1) {
        uint32_t p = spf[m];
        seq_t prime_contrib = (seq_t)p;
        sum += prime_contrib;

        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > 1) sum += (seq_t)exp;   /* add exponent if > 1 */
    }

    return sum;
}
