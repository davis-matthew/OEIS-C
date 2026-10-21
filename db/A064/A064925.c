/*
 * A064925  Period of continued fraction for sqrt(21)*n.
 * https://oeis.org/A064925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 7300, u64 n <= 7300,
 *            u128 n <= 7300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of continued fraction for sqrt(21)*n (A064925).
   The period of the simple continued fraction of
   sqrt(21 * n^2) is returned.  The algorithm is the standard
   one for quadratic surds:  a0 = floor(sqrt(N));
   then iterate
       m_{k+1} = a_k * d_k - m_k
       d_{k+1} = (N - m_{k+1}^2) / d_k
       a_{k+1} = (a0 + m_{k+1}) / d_{k+1}
   until a_{k+1} == 2*a0.
*/

static inline seq_t seq_isqrt(seq_t x)
{
    /* Integer square root: floor(sqrt(x)) */
    seq_t low = 0, high = x;
    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1); /* upper mid */
        /* mid*mid may overflow, so test via division */
        if (mid == 0 || mid <= x / mid)
            low = mid;           /* mid^2 <= x */
        else
            high = mid - 1;      /* mid^2  > x */
    }
    return low;
}

seq_t a(idx_t n)
{
    /* N = 21 * n^2;  n>=1 */
    seq_t N = (seq_t)n * (seq_t)n;
    N *= (seq_t)21;

    seq_t a0 = seq_isqrt(N);            /* floor(sqrt(N))   */
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a  = a0;
    seq_t period = 0;

    do {
        m = a * d - m;
        /* d divides (N - m*m) exactly */
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        period++;
    } while (a != 2 * a0);

    return period;
}
