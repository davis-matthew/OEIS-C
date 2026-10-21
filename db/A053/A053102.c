/*
 * A053102  a(n) = ((6*n+9)(!^6))/9(!^6), related to A034723 (((6*n+3)(!^6))/3
 *          sextic, or 6-factorials).
 * https://oeis.org/A053102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * (6*n + 9), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A053102.
 * For n = 0 the value is 1; otherwise the recurrence
 * a(n) = a(n-1) * (6*n + 9) is applied.
 */
seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* cast to seq_t to avoid overflows of the intermediate integer */
        result *= (seq_t)(6 * i + 9);
    }
    return result;
}
