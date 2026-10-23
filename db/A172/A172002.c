/*
 * A172002  A permutation of the natural numbers in groups of 2*k^2,
 *          k=1,2,....
 * https://oeis.org/A172002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) permutes the natural numbers in blocks of size 4*k^2 that
 *            are arranged as two rows of length 2*k^2, each row divided into
 *            two halves of length k^2. The numbers in each half are read
 *            alternating from the reversed first half and the natural second
 *            half. The sequence is obtained by concatenatin...
 * Range:     u8 n <= 220, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t cumulative(idx_t k)
{
    __int128 kk = k;
    __int128 val = 2 * kk * (kk + 1) * (2 * kk + 1) / 3;
    if (val > (__int128)UINT64_MAX) return UINT64_MAX;
    return (idx_t)val;
}

seq_t a(idx_t n)
{
    /* find k such that cumulative(k-1) < n <= cumulative(k) */
    idx_t low = 1, high = 1;
    while (cumulative(high) < n) {
        if (high >= UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cumulative(mid) < n) low = mid + 1;
        else high = mid;
    }
    idx_t k = low;

    idx_t prevCum = cumulative(k - 1); /* cumulative up to previous block */

    /* offset inside the current block (0‑based) */
    idx_t pos = n - prevCum - 1;

    idx_t rowLen = (idx_t)((__int128)2 * k * k); /* length of one row */
    idx_t halfSize = k * k;                       /* half size of a row */

    idx_t row = pos / rowLen;                     /* 0 or 1 */
    idx_t posInRow = pos % rowLen;
    idx_t col = posInRow >> 1;                    /* column index */

    idx_t offsetWithinRow;
    if ((posInRow & 1) == 0) {                    /* even position -> reversed first half */
        offsetWithinRow = (halfSize - 1) - col;
    } else {                                     /* odd position -> natural second half */
        offsetWithinRow = halfSize + col;
    }

    idx_t rowStartVal = prevCum + 1 + row * rowLen;

    idx_t valIdx = rowStartVal + offsetWithinRow;

    return (seq_t)valIdx;
}
