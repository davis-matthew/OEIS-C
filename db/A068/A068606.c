/*
 * A068606  Square table by antidiagonals of T(n,k)=n*k*(n+k+1).
 * https://oeis.org/A068606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i*j*(i+j+1), where (i,j) are obtained from the n-th
 *            element of the square table T(i,j)=i*j*(i+j+1) read by
 *            antidiagonals (i+j constant)
 * Range:     u8 n <= 58, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that the cumulative count
       of elements up to that diagonal exceeds n.  The cumulative
       count for diagonal s is (s+1)*(s+2)/2.  Because n is
       typically small (at most a few thousand) this loop is fast. */
    idx_t s = 0;
    uint64_t cum = 1; /* cumulative count for diagonal s */

    while (cum <= n) {
        ++s;
        cum += (s + 1);
    }

    /* Position of n within diagonal s */
    idx_t prev_cum = cum - (s + 1); /* cumulative count up to diagonal s-1 */
    idx_t t = n - prev_cum;        /* offset within diagonal s, 0 ≤ t ≤ s */

    /* Convert to seq_t for arithmetic. */
    seq_t i = (seq_t)t;
    seq_t j = (seq_t)(s - t);

    /* Compute the value T(i,j) = i * j * (i + j + 1). */
    return i * j * (i + j + 1);
}
