/*
 * A023857  a(n) = 1*(n+3-1) + 2*(n+3-2) + .... + k*(n+3-k), where
 *          k=floor((n+1)/2).
 * https://oeis.org/A023857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k+1)*(3*n+8-2*k)/6 where k = ceil(n/2)
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023857: a(n) = Sum_{i=1..ceil(n/2)} i*(n-i+3).  */
/* Offset 1: a(1) = 3. */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid accidental overflow when mixing types. */
    seq_t n_seq = (seq_t)n;

    /* k = ceil(n/2) = (n+1)/2 for unsigned integers. */
    seq_t k = (n_seq + 1) / 2;

    /* Compute the exact numerator: k*(k+1)*(3*n+8-2*k). */
    seq_t numerator = k * (k + 1) * ( (seq_t)3 * n_seq + 8 - (seq_t)2 * k );

    /* Division by 6 is exact as proven by the closed‑form identity. */
    return numerator / 6;
}
