/*
 * A194645  Number of ways to place 3n nonattacking kings on a 6 X 2n
 *          cylindrical chessboard.
 * https://oeis.org/A194645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2*4^n + 2*3^n + 4*(2+sqrt(2))^n + 4*(2-sqrt(2))^n + 2 *
 *            Recurrence: * a(1)=32, a(2)=100, a(3)=344, a(4)=1220, a(5)=4392
 *            * a(n)=24*a(n-5) - 86*a(n-4) + 104*a(n-3) - 53*a(n-2) +
 *            12*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;  /* maximum index currently computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1 */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* Allocate space for indices 0..5 (index 0 unused) */
        cache = (seq_t*)calloc(6, sizeof(seq_t));
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)100;
        cache[3] = (seq_t)344;
        cache[4] = (seq_t)1220;
        cache[5] = (seq_t)4392;
        cache_size = 5;
    }

    /* If requested index is already computed, return it */
    if (n <= cache_size) {
        return cache[n];
    }

    /* Extend cache */
    idx_t old_size = cache_size;
    cache_size = n;
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)(cache_size + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed; return 0 as a safe fallback */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute new values using the recurrence */
    for (idx_t i = old_size + 1; i <= n; ++i) {
        seq_t a1 = cache[i-5];
        a1 *= (seq_t)24;

        seq_t a2 = cache[i-3];
        a2 *= (seq_t)104;

        seq_t a3 = cache[i-1];
        a3 *= (seq_t)12;

        /* Sum of positive terms */
        seq_t sumA = a1 + a2 + a3;

        seq_t b1 = cache[i-4];
        b1 *= (seq_t)86;

        seq_t b2 = cache[i-2];
        b2 *= (seq_t)53;

        /* Sum of terms to subtract */
        seq_t sumB = b1 + b2;

        /* Final value (assumed sumA >= sumB for all n>=6) */
        cache[i] = sumA - sumB;
    }

    return cache[n];
}
