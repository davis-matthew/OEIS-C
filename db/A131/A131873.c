/*
 * A131873  Right-to-left partial row sums of triangle A131844.
 * https://oeis.org/A131873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = {5} if n = r(r+1)/2 + c with 0 ≤ c ≤ r, then if c = 0 ->
 *            a(n) = 7r + 1 else -> a(n) = 4r - c + 1 Here r =
 *            floor((sqrt(8n+1)-1)/2) is the row number, and c = n - r(r+1)/2
 *            is the column within that row (starting from 0)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A131873_H_
#define _A131873_H_

/* Compute A131873(n) for any nonnegative idx_t n. */
seq_t a(idx_t n)
{
    /* Find the row r using binary search on r such that r(r+1)/2 <= n. */
    idx_t low = 0;
    idx_t high = n;          /* r never exceeds n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;           /* candidate r */
        unsigned __int128 tri = (unsigned __int128)mid *
                                (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column within the row. */
    idx_t prev_tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - prev_tri;

    /* Compute the entry value using 64‑bit arithmetic. */
    uint64_t value;
    if (c == 0)
        value = 7 * (uint64_t)r + 1;
    else
        value = 4 * (uint64_t)r - (uint64_t)c + 1;

    return (seq_t)value;
}

#endif /* _A131873_H_ */
