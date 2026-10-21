/*
 * A201629  a(n) = n if n is even and otherwise its nearest multiple of 4.
 * https://oeis.org/A201629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even; otherwise its nearest multiple of 4. For
 *            odd n, a(n) = n-1 when n ≡ 1 (mod 4), else a(n) = n+1
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return (seq_t)n;
    } else {
        return (seq_t)((n & 3) == 1 ? (n - 1) : (n + 1));
    }
}
