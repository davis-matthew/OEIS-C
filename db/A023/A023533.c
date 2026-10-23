/*
 * A023533  a(n) = 1 if n is of the form m(m+1)(m+2)/6, and 0 otherwise.
 * https://oeis.org/A023533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = m(m+1)(m+2)/6 for some non‑negative integer m,
 *            otherwise 0. This implementation uses a binary search on the
 *            tetrahedral function m ↦ m(m+1)(m+2)/6, with 128‑bit arithmetic
 *            to avoid overflow
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline __int128 tetra_val(uint64_t m)
{
    return (__int128)m * (m + 1) * (m + 2) / 6;
}

seq_t a(idx_t n)
{
    /* Special case: tetrahedral number for m = 0 is 0. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Find an upper bound high such that tetra_val(high) > n. */
    uint64_t low  = 0;
    uint64_t high = 1;
    while (tetra_val(high) <= (__int128)n) {
        high <<= 1;
    }

    /* Binary search for the smallest m with tetra_val(m) > n. */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        __int128 val = tetra_val(mid);
        if (val <= (__int128)n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    /* low is now the first integer with tetra_val(low) > n.
       Check whether tetra_val(low-1) equals n. */
    if (low == 0) {
        return (seq_t)0;
    }
    __int128 val = tetra_val(low - 1);
    return (val == (__int128)n) ? (seq_t)1 : (seq_t)0;
}
