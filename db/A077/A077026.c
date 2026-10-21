/*
 * A077026  a(n) = Sum_{k=1..n} floor(n/k + 1)-floor(n/k + 1/2).
 * https://oeis.org/A077026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊n/k + 1⌋ – ⌊n/k + 1/2⌋
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;
        /* Count 1 if the fractional part of n/k is < 0.5,
           i.e., 2*r < k.  Use 128‑bit multiplication to avoid overflow. */
        if (((unsigned __int128)r << 1) < k)
            ++res;
    }
    return res;
}
