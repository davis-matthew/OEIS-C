/*
 * A183476  Number of n X 2 0..2 arrays with every element equal to either the
 *          sum mod 3 of its vertical neighbors or the sum mod 3 of its
 *          horizontal neighbors.
 * https://oeis.org/A183476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A183476
   a(n) = 3*a(n-1) + 12*a(n-3) - 18*a(n-4) - 36*a(n-6)
   with initial values
   a(1)=3, a(2)=15, a(3)=39, a(4)=135, a(5)=495, a(6)=1647
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to keep already computed values */
    static idx_t last_computed = 0;          /* largest n computed so far   */
    static seq_t *cache = NULL;              /* dynamic array of a(0..last) */

    /* ---------- Initialise cache for first six values ---------- */
    if (cache == NULL) {
        /* allocate space for indices 0..6 (index 0 unused) */
        cache = (seq_t *)realloc(NULL, 7 * sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failure */

        /* base values (index 0 is unused) */
        cache[0] = 0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)39;
        cache[4] = (seq_t)135;
        cache[5] = (seq_t)495;
        cache[6] = (seq_t)1647;

        last_computed = 6;
    }

    /* ---------- Handle requests for n <= 0 or already computed ---------- */
    if (n == 0) {
        return 0;    /* not part of the OEIS sequence, but harmless */
    }
    if (n <= last_computed) {
        return cache[n];
    }

    /* ---------- Grow the cache to hold values up to n ---------- */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        return 0;    /* allocation failure */
    }
    cache = tmp;

    /* ---------- Compute missing terms using the recurrence ---------- */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 6) {                     /* safety: should not happen */
            cache[i] = (seq_t)0;
        } else {
            seq_t term1 = 3 * cache[i - 1];
            seq_t term2 = 12 * cache[i - 3];
            seq_t term3 = 18 * cache[i - 4];
            seq_t term4 = 36 * cache[i - 6];
            seq_t sum_pos  = term1 + term2;
            seq_t sum_neg  = term3 + term4;
            cache[i] = sum_pos - sum_neg;   /* guarantees non‑negative result */
        }
    }
    last_computed = n;

    return cache[n];
}
