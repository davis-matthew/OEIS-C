/*
 * A120879  G.f. satisfies: A(x) = A(x^3)*(1 + 3*x + 2*x^2).
 * https://oeis.org/A120879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 3^(# of 1's in the ternary expansion of n) * * 2^(# of
 *            2's in the ternary expansion of n)
 * Range:     u8 n <= 363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1, a(n)=a(n/3)*3^((n%3)%2)*2^((n%3)/2). */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    while (n > 0) {
        idx_t rem = n % (idx_t)3;
        if (rem == 1)
            res *= (seq_t)3;
        else if (rem == 2)
            res *= (seq_t)2;
        n /= (idx_t)3;
    }
    return res;
}
