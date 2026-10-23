/*
 * A025752  7th-order Patalan numbers (generalization of Catalan numbers).
 * https://oeis.org/A025752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) * (49*n - 56) / n for n >= 2
 *            This recurrence follows from the definition a(n) =
 *            7^(n-1)*(49*n-56)!/(n! ... ) and yields integer values for all n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below iteratively applies the recurrence, using 128‑bit
   arithmetic for 8‑byte seq_t types to avoid overflow in the intermediate
   product. */

seq_t a(idx_t n)
{
    /* handle zero and one directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;   /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* numerator = 49*i - 56 */
        seq_t num = (seq_t)(49 * i - 56);

        /* multiply by num and divide by i */
        if (sizeof(seq_t) <= 8)   /* 64‑bit or smaller */
        {
            unsigned __int128 tmp = (unsigned __int128)res * (unsigned __int128)num;
            res = (seq_t)(tmp / i);
        }
        else                       /* arbitrary precision; the class handles overflow */
        {
            seq_t tmp = res * num;
            res = tmp / (seq_t)i;
        }
    }
    return res;
}
