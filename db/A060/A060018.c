/*
 * A060018  a(n) = floor(2*sqrt(n-2)).
 * https://oeis.org/A060018
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*sqrt(n-2)) = floor(sqrt(4*n-8))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 2; for n < 2 the result is 0. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* big = 4*n-8, computed in 128‑bit to avoid overflow          */
    unsigned __int128 big = (unsigned __int128)n * 4u - 8u;

    /* binary search for floor(sqrt(big)).  sqrt(max 4*n-8) <= 2^33
       so 1<<34 is a safe upper bound.                              */
    unsigned __int128 low  = 0;
    unsigned __int128 high = (unsigned __int128)1 << 34;  /* 2^34  */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= big)
            low = mid;
        else
            high = mid - 1;
    }

    return (seq_t)low;
}
