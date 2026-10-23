/*
 * A187203  The bottom entry in the absolute difference triangle of the
 *          divisors of n.
 * https://oeis.org/A187203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the bottom entry of the absolute difference triangle of the divisors of n (OEIS A187203) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: floor square root of an unsigned 64‑bit integer via binary search.
   Uses 128‑bit arithmetic to avoid overflow when squaring mid. */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * (mid);
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (outside OEIS offset). */
    if (n == 0)
        return 0;

    /* Maximum number of divisors for n up to 10^12 is < 2000,
       so this stack array is safe for the required range. */
    const size_t MAX_DIV = 4096;
    idx_t divs[MAX_DIV];
    size_t dcnt = 0;

    idx_t sqrt_n = isqrt(n);
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i)
                divs[dcnt++] = n / i;
        }
    }

    /* Sort divisors in ascending order (simple insertion sort). */
    for (size_t i = 1; i < dcnt; ++i) {
        idx_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* The absolute difference triangle.  We reuse 'divs' to store the current
       row and overwrite it in place. */
    size_t len = dcnt;
    while (len > 1) {
        for (size_t i = 0; i + 1 < len; ++i) {
            idx_t a_val = divs[i];
            idx_t b_val = divs[i + 1];
            divs[i] = (a_val > b_val) ? (a_val - b_val) : (b_val - a_val);
        }
        --len;
    }

    return (seq_t)divs[0];
}
