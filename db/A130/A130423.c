/*
 * A130423  Main diagonal of array A[k,n] = n-th sum of 3 consecutive k-gonal
 *          numbers, k>2.
 * https://oeis.org/A130423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n^2 - 3*n + 8)/2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is zero‑based? No, sequence starts at n=1, so just use n as given. */
    seq_t ln = (seq_t)n;      /* convert index to seq_t once */
    seq_t n2 = ln * ln;       /* n^2 */
    seq_t three_n2 = n2 * (seq_t)3; /* 3*n^2 */
    seq_t three_n = ln * (seq_t)3;   /* 3*n */
    seq_t temp = three_n2 - three_n; /* 3*n^2 - 3*n */
    temp = temp + (seq_t)8;          /* +8 */
    seq_t result = ln * temp;        /* n * (3*n^2 - 3*n + 8) */
    result = result / (seq_t)2;      /* divide by 2 */
    return result;
}
