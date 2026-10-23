/*
 * A047374  Numbers that are congruent to {4, 5} mod 7.
 * https://oeis.org/A047374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n - 5*(-1)^n - 3)/4 (offset 1)
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1, so all intermediate arithmetic is safe for the values
       that fit in seq_t.  Use 128‑bit intermediates to avoid overflow */
    if (n & 1ULL) {            /* odd n : a = 4 + 7·(n‑1)/2 */
        unsigned __int128 k = ((unsigned __int128)(n - 1)) / 2;
        unsigned __int128 v = k * 7 + 4;
        return (seq_t)v;
    } else {                   /* even n : a = 5 + 7·(n/2 – 1) */
        unsigned __int128 k = (unsigned __int128)(n / 2 - 1);
        unsigned __int128 v = k * 7 + 5;
        return (seq_t)v;
    }
}
