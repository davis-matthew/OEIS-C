/*
 * A101223  Numbers m whose deficiency is 10, or: sigma(m) = 2m - 10.
 * https://oeis.org/A101223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   isprime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101223: numbers m with deficiency 10 (sigma(m) = 2m – 10).
   a(1) = 11,
   a(2) = 21,
   for n ≥ 3, a(n) = 2^(k-1)*(2^k + 9) where k is the (n‑2)-th integer such
   that 2^k + 9 is prime.  The indices k for which 2^k+9 is prime are
   listed in OEIS A057196.  The function generates terms on demand,
   caching intermediate values so subsequent calls are fast.   */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/isprime_big.h"

static idx_t max_n_computed = 0;      /* number of terms already stored   */
static seq_t *table = NULL;           /* cache of computed terms           */
static idx_t table_size = 0;          /* allocated size of the cache       */
static idx_t next_k = 2;              /* next k to test for 2^k+9 prime     */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n<1, return 0      */

    /* Lazy initialization of cache with the first two known terms.   */
    if (table == NULL) {
        table_size = 16;
        table = (seq_t *)malloc(sizeof(seq_t) * table_size);
        table[0] = (seq_t)11;   /* a(1) */
        table[1] = (seq_t)21;   /* a(2) */
        max_n_computed = 2;
        next_k = 2;               /* begin testing from k=2           */
    }

    /* Generate terms until we have at least the requested n.          */
    while (max_n_computed < n) {
        /* Find the next k such that 2^k + 9 is prime.                  */
        while (true) {
            seq_t p = pow2(next_k) + (seq_t)9;   /* 2^k + 9 */
            if (isprime_big(p)) {
                seq_t m = pow2(next_k - 1) * p;   /* 2^(k-1)*(2^k+9) */
                if (max_n_computed >= table_size) {
                    table_size *= 2;
                    table = (seq_t *)realloc(table, sizeof(seq_t) * table_size);
                }
                table[max_n_computed++] = m;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }

    return table[n - 1];
}
