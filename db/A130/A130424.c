/*
 * A130424  Main diagonal of array A[k,n] = n-th sum of k consecutive k-gonal
 *          numbers, k>2.
 * https://oeis.org/A130424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)*(7*n^3-8*n^2+12*n-3)/6
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 245, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not defined for n=0 but safe */
    seq_t sn   = (seq_t)n;
    seq_t nsq  = sn * sn;                    /* n^2 */
    seq_t ncub = nsq * sn;                   /* n^3 */
    seq_t term = 7 * ncub - 8 * nsq + 12 * sn - 3;
    seq_t num  = (sn + 2) * term;            /* numerator */
    return num / 6;                          /* integer division */
}
