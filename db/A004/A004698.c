/*
 * A004698  a(n) = floor(Fibonacci(n)/5).
 * https://oeis.org/A004698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(Fibonacci(n)/5), computed by carrying F(n) = 5*q(n)
 *            + r(n), 0 <= r(n) < 5, through the recurrence F(n) = F(n-1) +
 *            F(n-2), F(0) = 0, F(1) = 1
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 51, u64 n <= 97, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t qa = 0, qb = 0;   /* quotients of F(i), F(i+1) by 5 */
    unsigned ra = 0, rb = 1; /* remainders of F(i), F(i+1) mod 5 */

    for (idx_t i = 0; i < n; i++) {
        unsigned s = ra + rb;
        seq_t qn = qa + qb;
        unsigned rn = s;
        if (rn >= 5) {
            rn -= 5;
            qn += 1;
        }
        qa = qb;
        ra = rb;
        qb = qn;
        rb = rn;
    }

    return qa;
}
