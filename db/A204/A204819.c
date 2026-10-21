/*
 * A204819  Final nonzero digit of n^n in base 9.
 * https://oeis.org/A204819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = final nonzero digit of n^n in base 9 The digit is
 *            computed as let vp3 = number of factors of 3 in n; let rem = (n
 *            / 3^vp3)^n mod 9 // n with all 3's removed, then powered let r =
 *            rem if (n even or vp3 even) else 3*rem mod 9 (rem is the part of
 *            n^n with all 3's removed, so its value mod 9 i...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* conventionally 0^0 = 1 */
        return (seq_t)1;

    /* compute vp3 = exponent of 3 in n */
    idx_t tmp = n;
    int vp3 = 0;
    while ((tmp % 3) == 0)
    {
        tmp /= 3;
        vp3++;
    }

    /* rem = (n / 3^vp3)^n mod 9 */
    seq_t base_rem = (seq_t)(tmp % 9);   /* n with all 3's removed, reduced mod 9 */

    /* modular exponentiation modulo 9 */
    seq_t rem = 1 % 9;
    seq_t pow = base_rem % 9;
    idx_t e   = n;
    while (e)
    {
        if (e & 1)
            rem = (rem * pow) % 9;
        pow = (pow * pow) % 9;
        e >>= 1;
    }

    /* final digit, possibly multiplied by an extra 3 */
    seq_t result = rem;
    if ((n & 1) && (vp3 & 1))
        result = (3 * rem) % 9;   /* rem already < 9, product < 27 */

    return result;
}
