/*
 * A048493  a(n) = (n+1)*2^n - n.
 * https://oeis.org/A048493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^n - n
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in a loop to avoid dependency on power routines */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 <<= 1;                     /* shift left by 1 each iteration */
    }

    seq_t result = (seq_t)(n + 1) * pow2; /* (n+1) * 2^n */
    result -= (seq_t)n;                   /* subtract n */
    return result;
}
