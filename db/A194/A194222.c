/*
 * A194222  a(n) = floor(Sum_{k=1..n} frac(k/5)), where frac() = fractional
 *          part.
 * https://oeis.org/A194222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/5) + floor((n+2)/5)
 * Range:     u8 n <= 357, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A194222: a(n) = floor(∑_{k=1..n} frac(k/5)) */
seq_t a(idx_t n)
{
    /* Convert index to seq_t first to avoid overflow during integer arithmetic. */
    seq_t a1 = (seq_t)(n + 1);
    seq_t a2 = (seq_t)(n + 2);
    return a1 / (seq_t)5 + a2 / (seq_t)5;
}
