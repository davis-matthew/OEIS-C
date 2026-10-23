/*
 * A177187  Union of A057080 and A001090.
 * https://oeis.org/A177187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1; for n >= 2, if n is odd: a(n) = 10*a(n-1) -
 *            a(n-2); if n is even: a(n) = a(n-1) - a(n-2);
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    if (n == 1u)
        return 1;

    seq_t a_prev2 = 0;   /* a(n-2) */
    seq_t a_prev1 = 1;   /* a(n-1) */
    seq_t a_curr;
    const seq_t TEN = (seq_t)10;

    for (idx_t i = 2u; i <= n; ++i) {
        if ((i & 1u) != 0u)          /* n is odd */
            a_curr = TEN * a_prev1 - a_prev2;
        else                         /* n is even */
            a_curr = a_prev1 - a_prev2;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
