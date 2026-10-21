/*
 * A086330  a(n) = Sum_{m >= 2} m! mod n.
 * https://oeis.org/A086330
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{m=2}^{n-1} (m! mod n)
 * (the sum is over the multiplicative remainders, not modulo the sum)
 */
seq_t a(idx_t n)
{
    if (n < 2u) return 0;   /* undefined for n<2, but return 0 */
    seq_t sum   = 0;
    seq_t fact  = 1;
    seq_t modn  = (seq_t)n;

    /* compute factorial modulo n and accumulate the remainders */
    for (idx_t m = 2u; m < n; ++m)
    {
        fact = (fact * (seq_t)m) % modn; /* m! mod n */
        sum += fact;                     /* add the remainder */
    }
    return sum;
}
