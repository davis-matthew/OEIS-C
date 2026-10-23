/*
 * A171769  Partial sums of A042964 (numbers congruent to 2 or 3 mod 4).
 * https://oeis.org/A171769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2*n+1)*n/2)
 * Range:     u8 n <= 11, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic to avoid 64‑bit overflow */
    seq_t N  = (seq_t)n;
    seq_t num = N * ((seq_t)2 * N + 1);   /* numerator: n*(2n+1) */
    seq_t res = num / 2 + (num & 1);        /* ceil division by 2 */
    return res;
}
