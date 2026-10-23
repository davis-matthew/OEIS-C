/*
 * A158192  Prime numbers p such that first digit of p = largest digit of p -
 *          smallest digit of p.
 * https://oeis.org/A158192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 301, u32 n <= 540, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158192: primes p such that first digit of p = largest digit of p - smallest digit of p
   The function builds the list lazily, caching the qualifying primes. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes */
    static seq_t *qual = NULL;     /* array of qualifying primes   */
    static size_t qual_cap = 0;    /* capacity of qual array       */
    static size_t qual_cnt = 0;    /* number of qualifying primes  */
    static seq_t next = 2;         /* next candidate prime         */

    /* Allocate (or grow) the cache array */
    if (!qual_cap) {
        qual_cap = 32;
        qual = (seq_t*)malloc(qual_cap * sizeof(seq_t));
        if (!qual) return 0; /* out‑of‑memory fallback */
    }

    /* Generate primes until we have at least n qualifying ones */
    while (qual_cnt < n) {
        if (is_prime_big(next)) {
            /* Compute first digit, max digit, min digit */
            seq_t tmp = next;
            int first = 0;
            /* most significant digit */
            while (tmp >= 10) tmp /= 10;
            first = (int)tmp;

            int dmin = 9, dmax = 0;
            tmp = next;
            /* iterate over all digits */
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                if (d < dmin) dmin = d;
                if (d > dmax) dmax = d;
                tmp /= 10;
            }

            if (first == dmax - dmin) {
                /* store the qualifying prime */
                if (qual_cnt == qual_cap) {
                    size_t new_cap = qual_cap * 2;
                    seq_t *tmp_ptr = (seq_t*)realloc(qual, new_cap * sizeof(seq_t));
                    if (!tmp_ptr) return 0; /* out‑of‑memory fallback */
                    qual = tmp_ptr;
                    qual_cap = new_cap;
                }
                qual[qual_cnt++] = next;
            }
        }
        ++next;
    }

    return qual[n - 1];
}
