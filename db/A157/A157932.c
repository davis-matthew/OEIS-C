/*
 * A157932  Numbers k such that (3^(35*k) + 5^(21*k) + 7^(15*k)) mod 105 is
 *          prime.
 * https://oeis.org/A157932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 3 if n is odd, * a(n) = 3*n - 2 if n ≡ 2 (mod 4), *
 *            a(n) = 3*n - 4 if n ≡ 0 (mod 4)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below offset; behaviour irrelevant */

    seq_t base = (seq_t)3 * (seq_t)n;
    seq_t sub;

    if ((n & 1) == 1)        /* odd */
        sub = (seq_t)3;
    else if ((n & 3) == 2)   /* n ≡ 2 mod 4 */
        sub = (seq_t)2;
    else                     /* n ≡ 0 mod 4 */
        sub = (seq_t)4;

    return base - sub;
}
