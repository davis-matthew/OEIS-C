/*
 * A111755  Excess of n over a greedy sum of distinct squares.
 * https://oeis.org/A111755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: compute OEIS A111755, excess of n over a greedy sum
 * of distinct squares.  For a given n the algorithm repeatedly
 * subtracts the largest unused square ≤ current value.  The process
 * stops when the value reaches 0 or when the square 1 has been
 * subtracted.  The remaining value (0‑3) is returned.
 *
 * This implementation uses an array mark to remember which squares
 * have already been used.  It works for any seq_t/idx_t pair
 * compliant with the build requirements. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Integer square root: floor(sqrt(x)).  Works for 0 ≤ x ≤ ULLONG_MAX. */
static inline idx_t
isqrt(idx_t x)
{
    idx_t low = 0;
    idx_t high = (x < 0xffffffffULL) ? x : 0xffffffffULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)x) {
            return mid;
        } else if (sq < (unsigned __int128)x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* prevent underflow */
            high = mid - 1;
        }
    }
    return high;
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset is 1; safe fallback */

    idx_t curr = n;
    idx_t maxk = isqrt(n);

    /* Allocate an array to record which squares k^2 have been used.
     * The array is indexed by k, 0..maxk; index 0 is unused.  Cast
     * the result of malloc for C++ compatibility. */
    uint8_t *used = (uint8_t *)malloc((size_t)(maxk + 1));
    if (!used) return (seq_t)0;          /* memory failure fallback */
    memset(used, 0, (size_t)(maxk + 1));

    while (curr > 0) {
        idx_t k = isqrt(curr);
        /* Skip any already‑used squares.  k>0 because curr>0. */
        while (k > 0 && used[k]) {
            --k;
        }
        if (k == 0) break;              /* should not happen */

        if (k == 1) {
            curr -= 1;                   /* subtract 1 and stop */
            break;
        }
        used[k] = 1;                     /* mark this square as used */
        curr -= k * k;
    }

    free(used);
    return (seq_t)curr;
}
