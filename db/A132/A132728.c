/*
 * A132728  Triangle T(n, k) = 4 - 3*(-1)^k, read by rows.
 * https://oeis.org/A132728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, less
 * Formula:   a(n) = 1 if the position index of a(n) in its row is even, else
 *            7. The position index starts at 0 for each row. The entire
 *            sequence is constructed by reading the triangle T(n,k) =
 *            4-3*(-1)^k row by row, starting with row 0
 * Range:     u8 n <= 495, u16 n <= 495, u32 n <= 495, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned __int128 value.
   Returns floor(sqrt(x)).  Uses binary search; result fits in 64 bits. */
static inline uint64_t floor_sqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0, high = (uint64_t)1 << 34;   /* sqrt(8*(2^64-1)+1) < 2^34 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 midsq = (unsigned __int128)mid * mid;
        if (midsq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return a(n) for the OEIS sequence A132728.
   n is the zero‑based index in the sequence (idx_t is uint64_t). */
seq_t a(idx_t n)
{
    /* Compute s = floor(sqrt(8*n + 1)) using 128‑bit arithmetic. */
    unsigned __int128 val = (unsigned __int128)n * (unsigned __int128)8 + 1;
    uint64_t s = floor_sqrt_u128(val);

    /* Row index r = floor((s - 1)/2). */
    uint64_t r = (s - 1) >> 1;

    /* Cumulative count up to the previous row: r*(r+1)/2. */
    unsigned __int128 cumPrev128 = (__int128)r * (r + 1) / 2;
    uint64_t cumPrev = (uint64_t)cumPrev128;

    /* Column index within the row. */
    uint64_t k = n - cumPrev;

    /* Triangle entry: 1 if k even, 7 if k odd. */
    return (seq_t)((k & 1) ? 7 : 1);
}
