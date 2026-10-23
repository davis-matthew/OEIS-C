/*
 * A188134  a(4*n) = n, a(1+2*n) = 4+8*n, a(2+4*n) = 2+4*n.
 * https://oeis.org/A188134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/4 if n % 4 == 0 a(n) = n if n % 4 == 2 a(n) = 4 * n if
 *            n is odd (These are equivalent to the OEIS definition a(4*n) =
 *            n, a(1+2*n) = 4+8*n, a(2+4*n) = 2+4*n)
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod4 = n & 3ULL;               /* compute n % 4 efficiently */
    if (mod4 == 0ULL)                   /* n ≡ 0 (mod 4) */
        return (seq_t)(n / 4ULL);
    else if (mod4 == 2ULL)              /* n ≡ 2 (mod 4) */
        return (seq_t)n;
    else                                /* n is odd (≡ 1 (mod 2)) */
        return (seq_t)(4ULL * n);
}
