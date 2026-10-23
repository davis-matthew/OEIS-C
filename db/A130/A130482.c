/*
 * A130482  a(n) = Sum_{k=0..n} (k mod 4) (Partial sums of A010873).
 * https://oeis.org/A130482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n/2 + (n mod 2)*((n-1) mod 4) - (n mod 2)/2
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* base term 3*n/2 */
    seq_t base = (seq_t)3 * n_seq / 2;

    /* n mod 2 */
    seq_t n_mod2 = n_seq & 1;

    /* (n-1) mod 4  == (n+3) mod 4  (avoids underflow) */
    seq_t n_minus1_mod4 = (n_seq + 3) & 3;

    /* correction term */
    seq_t t = n_mod2 * n_minus1_mod4 - (n_mod2 / 2);

    return base + t;
}
