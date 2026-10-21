/*
 * A212201  Records in A212200.
 * https://oeis.org/A212201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(2^(k+1)) - 1 if n is odd (k = (n-1)/2), a(n) =
 *            (2^(2^(k+1)) - 1) / 3 if n is even (k = n/2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* first term */
    if (n == 2) return (seq_t)3;   /* second term */

    idx_t k = (n & 1) ? (n - 1) / 2 : n / 2;   /* floor(n/2), adjusted for parity */
    seq_t val = (seq_t)2;                       /* start with 2 = 2^1 */

    /* Compute val = 2^(2^(k+1)) by repeated squaring.
       After t squarings, val = 2^(2^t).  We need t = k+1. */
    for (idx_t i = 0; i < k + 1; ++i) {
        val = val * val;
    }

    seq_t res = val - (seq_t)1;  /* 2^(2^(k+1)) - 1 */

    if (n & 1)  /* odd n */
        return res;
    else        /* even n, divide by 3 (always divisible) */
        return res / (seq_t)3;
}
