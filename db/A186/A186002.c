/*
 * A186002  Hankel transform of A186001.
 * https://oeis.org/A186002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n+1) = a(n) * 2^n * n!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A186002. */
seq_t a(idx_t n)
{
    /* Initialize a(0) = 1 */
    seq_t result = 1;     /* current a(k)     */
    seq_t factorial = 1;  /* k! for current k */
    seq_t two_pow   = 1;  /* 2^k for current k */

    /* Iterate for k = 0 .. n-1 to compute a(n) */
    for (idx_t k = 0; k < n; ++k) {
        /* result = a(k) * 2^k * k!  -> now a(k+1)  */
        result *= two_pow;
        result *= factorial;

        /* Prepare for next iteration: (k+1)! and 2^(k+1) */
        factorial *= (seq_t)(k + 1);
        two_pow   *= 2;
    }

    return result;
}
