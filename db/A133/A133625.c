/*
 * A133625  Binomial(n+p, n) mod n where p=5.
 * https://oeis.org/A133625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) mod n
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute C(n+5,5) exactly with successive integer divisions
       and then return the result modulo n.  The intermediate
       values are small enough for the specified build limits
       (n ≤ 10 000 easily fits in 64 bit). */
    seq_t r = 1;
    for (idx_t i = 1; i <= 5; ++i)
    {
        seq_t term = (seq_t)n + (seq_t)i;   /* n+i */
        r = (r * term) / (seq_t)i;          /* exact division */
    }
    return r % (seq_t)n;
}
