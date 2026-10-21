/*
 * A082575  Nonnegative numbers in (3*A005836) union (3*A005836 - 2) [A005836
 *          lists the numbers with base-3 representation containing no 2].
 * https://oeis.org/A082575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0 For n >= 2: i = ⌊n/2⌋ B(i) = Σ (i bit j) * 3^j a(n) =
 *            3*B(i) if n is odd a(n) = 3*B(i) - 2 if n is even (Thus a(n) =
 *            3*B(⌊n/2⌋) – 2*δ_{n even}, n≥2) where B(i) is the number
 *            obtained by interpreting the binary representation of i as a
 *            base‑3 number
 * Range:     u8 n <= 39, u16 n <= 1279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;           // first term
    idx_t i = n / 2;                // integer division

    /* compute B(i) */
    seq_t b = 0;
    seq_t pow3 = 1;                 // 3^0 initially
    idx_t t = i;
    while (t)
    {
        if (t & 1)
            b += pow3;              // add 3^position if bit is 1
        pow3 *= 3;                  // next power of 3
        t >>= 1;
    }

    if ((n & 1) == 0)               // n even: use 3*B(i) - 2
        return (seq_t)3 * b - 2;
    else                            // n odd: use 3*B(i)
        return (seq_t)3 * b;
}
