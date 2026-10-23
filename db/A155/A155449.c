/*
 * A155449  Numbers k == 6 or 11 (mod 17).
 * https://oeis.org/A155449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 + 17 * ((n-1)/2) if n is odd * a(n) = 11 + 17 *
 *            ((n-2)/2) if n is even * (OEIS A155449 – numbers k such that k^2
 *            ≡ 2 (mod 17) with a(1)=6)
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    if (n & 1)  /* odd index */
    {
        seq_t t = (seq_t)(n - 1) / 2;
        return 6 + 17 * t;
    }
    else        /* even index */
    {
        seq_t t = (seq_t)(n - 2) / 2;
        return 11 + 17 * t;
    }
}
