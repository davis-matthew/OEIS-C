/*
 * A077429  a(n) = floor(log_10(n^2)).
 * https://oeis.org/A077429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_10(n^2))
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^2 in the target type */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Find the largest k such that 10^(k+1) > n^2.
       The count k is the desired floor(log10(n^2)). */
    seq_t p   = (seq_t)10;  /* current power of 10: 10^(k+1) */
    seq_t e   = 0;          /* k, the answer */

    while (sq >= p) {
        p *= (seq_t)10;
        e++;
    }

    return e;
}
