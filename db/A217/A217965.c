/*
 * A217965  Number of 2Xn arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..2 2Xn array.
 * https://oeis.org/A217965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +7*a(n-2) +6*a(n-3) -2*a(n-4) -10*a(n-5)
 *            -11*a(n-6) -32*a(n-7) -23*a(n-8) -16*a(n-9) +22*a(n-10)
 *            +4*a(n-11) Initial values: a(1)=9, a(2)=17, a(3)=91, a(4)=321,
 *            a(5)=1341, a(6)=5269, a(7)=21379, a(8)=85633, a(9)=344955,
 *            a(10)=1386679, a(11)=5579199 a(1) is the first term (OEIS
 *            offs...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated slots (max index + 1)   */
    static idx_t  last  = 0;   /* highest index that is valid */

    /* Initial values for n = 1 … 11.  The array size is a compile‑time constant. */
    static const seq_t init_vals[11] = {
        9ULL, 17ULL, 91ULL, 321ULL, 1341ULL, 5269ULL,
        21379ULL, 85633ULL, 344955ULL, 1386679ULL, 5579199ULL
    };

    /* If the requested value is already cached, return it. */
    if (n <= last) {
        return cache[n];
    }

    /* Ensure the cache has enough space for index n. */
    if (n + 1 > cap) {
        idx_t new_cap = (n + 1 > 128) ? n + 1 : 128;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: abort the program. */
            abort();
        }
        cache = (seq_t *)tmp;
        cap   = new_cap;
    }

    /* Compute all missing indices up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 11) {
            /* Use the known initial values. */
            cache[i] = init_vals[i - 1];
        } else {
            /* Recurrence: all indices i‑1 … i‑11 are already computed. */
            seq_t t1 = 2 * cache[i - 1];
            seq_t t2 = 7 * cache[i - 2];
            seq_t t3 = 6 * cache[i - 3];
            seq_t t4 = 2 * cache[i - 4];
            seq_t t5 = 10 * cache[i - 5];
            seq_t t6 = 11 * cache[i - 6];
            seq_t t7 = 32 * cache[i - 7];
            seq_t t8 = 23 * cache[i - 8];
            seq_t t9 = 16 * cache[i - 9];
            seq_t t10 = 22 * cache[i - 10];
            seq_t t11 = 4 * cache[i - 11];

            cache[i] = t1 + t2 + t3 - t4 - t5 - t6 - t7 - t8 - t9 + t10 + t11;
        }
    }

    last = n;
    return cache[n];
}
