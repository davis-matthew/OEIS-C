/*
 * A176748  a(n) = a(n-1)^3-a(n-2) with a(1)=1, a(2)=2.
 * https://oeis.org/A176748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^3 - a(n-2), a(1)=1, a(2)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache large enough for n ≤ 20000, adjust if needed */
    static seq_t cache[20000];
    static size_t sz = 0;   /* number of terms currently stored */

    if (n <= 0)          /* sequence is defined only for n ≥ 1 */
        return 0;

    /* initialise base cases on the first use */
    if (sz == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        sz = 2;
    }

    /* compute terms up to n, if not already computed */
    if ((idx_t)sz < n) {
        for (size_t i = sz; i < (size_t)n; ++i) {
            seq_t v1 = cache[i - 1];   /* a(i)   */
            seq_t v2 = cache[i - 2];   /* a(i-1) */
            seq_t cube = v1 * v1 * v1; /* a(i)^3 */
            cache[i] = cube - v2;      /* a(i+1) = a(i)^3 - a(i-1) */
        }
        sz = (size_t)n;
    }

    return cache[n - 1];
}
