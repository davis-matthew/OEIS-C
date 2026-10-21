/*
 * A204372  Expansion of phi(x)^2 * (5 * phi(-x)^8 + 64 * x * psi(-x)^8) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A204372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=5, a(n)=4*∑_{d odd|n} (-1)^{(d-1)/2} d^4 for n>0
 *            (equivalently: a(n)=4*∑_{d odd|n} s(d) d^4 where s(d)=+1 if
 *            d≡1(4), -1 if d≡3(4))
 * Range:     i8 n <= 2, i16 n <= 10, i32 n <= 152, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; d += 2) {          /* only odd divisors */
        if (n % d == 0) {
            seq_t d2 = (seq_t)d * (seq_t)d;      /* d^2 */
            seq_t d4 = d2 * d2;                  /* d^4 */
            if (d % 4 == 1)                      /* s(d)=+1 */
                sum += d4;
            else                                 /* d%4==3, s(d)=-1 */
                sum -= d4;
        }
    }
    return sum * 4;
}
