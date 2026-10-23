/*
 * A100058  Expansion of 1 / (1 - 3x - x^2 + 2x^3).
 * https://oeis.org/A100058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3); initial values: a(0)=1,
 *            a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 20000  /* sufficient for n up to about 10000 */

static seq_t cache[MAX_N + 1];
static idx_t computed = 0;   /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n > MAX_N) {
        /* For n larger than the preallocated array: compute on the fly
           using a simple recursion; this path should not be hit for
           the intended usage limits. */
        if (n == 0) return 1;
        if (n == 1) return 3;
        if (n == 2) return 10;
        seq_t a_nm1 = a(n-1);
        seq_t a_nm2 = a(n-2);
        seq_t a_nm3 = a(n-3);
        return 3 * a_nm1 + a_nm2 - 2 * a_nm3;
    }

    /* Initialize base cases lazily on the first call */
    if (computed < 2) {
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 10;
        computed = 2;
    }

    if (n <= computed)
        return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] + cache[i-2] - 2 * cache[i-3];
    }
    computed = n;
    return cache[n];
}
