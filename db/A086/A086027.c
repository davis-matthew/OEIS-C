/*
 * A086027  a(n) = Sum_{i=1..n} binomial(i+5,6)^2.
 * https://oeis.org/A086027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=1..n} binomial(i+5,6)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 98, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by iteratively updating C(i+5,6) using
   the relation C(i+5,6) = C(i+4,6) * (i+5)/(i-1). */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t c   = (seq_t)0;  /* current binomial value */

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            c = (seq_t)1;                     /* C(6,6) = 1 */
        } else {
            c = c * (seq_t)(i + 5) / (seq_t)(i - 1);
        }
        sum += c * c;                          /* add C(i+5,6)^2 */
    }

    return sum;
}
