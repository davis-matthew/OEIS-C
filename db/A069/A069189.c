/*
 * A069189  Numbers k such that A007913(k) < sqrt(k).
 * https://oeis.org/A069189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 3317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069189: numbers k such that A007913(k) < sqrt(k)
   Computed by a linear search using prime factorisation with a
   smallest‑prime‑factor table. The first 24 terms are:
   4, 8, 9, 12, 16, 18, 25, 27, 32, 36, 45, 48, 49, 50, 54, 63, 64,
   72, 75, 80, 81, 96, 98, 100.
*/
#include <stdint.h>
#include <stdlib.h>

/* Smallest–prime–factor table */
static uint64_t  *spf      = NULL; /* array of spf values */
static uint64_t  spf_limit = 0;    /* current size of the table (last index) */

/* Rebuild the spf table up to 'limit' (inclusive) */
static void build_spf(uint64_t limit)
{
    /* free existing table */
    if (spf != NULL) {
        free(spf);
    }
    /* allocate new table and initialise */
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= limit; ++i) {
        spf[i] = i;
    }

    /* sieve for spf */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Return the n-th term of A069189 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* guard for offset 1 */
    }

    /* initialise spf table once to a reasonable size */
    static int spf_inited = 0;
    if (!spf_inited) {
        build_spf(1000000);           /* arbitrary starting limit */
        spf_inited = 1;
    }

    seq_t  count = 0;                 /* number of qualifying k found */
    idx_t  k     = 1;                 /* integer being tested */

    for (;;) {
        /* extend spf table if needed */
        if ((uint64_t)k > spf_limit) {
            uint64_t new_limit = spf_limit * 2 + 1;
            if (new_limit < (uint64_t)k) {
                new_limit = k;
            }
            build_spf(new_limit);
        }

        /* factor k using the spf table */
        uint64_t temp = k;
        seq_t s = 1;                 /* square‑free part (A007913) */
        seq_t d2 = 1;                /* square part (A008833) */
        while (temp > 1) {
            uint64_t p = spf[temp];
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t p_seq = (seq_t)p;

            if (e >= 2) {            /* contribute to square part */
                seq_t pe_even = 1;
                int even_exp = e - (e & 1);   /* greatest even <= e */
                for (int i = 0; i < even_exp; ++i) {
                    pe_even *= p_seq;
                }
                d2 *= pe_even;
            }

            if (e & 1) {             /* odd exponent goes to square‑free part */
                s *= p_seq;
            }
        }

        if (s < d2) {                 /* satisfies the condition */
            ++count;
            if (count == (seq_t)n) {
                return (seq_t)k;       /* nth term found */
            }
        }

        ++k;
    }
}
