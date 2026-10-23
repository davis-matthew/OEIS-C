/*
 * A190207  a(n) = [n*u + n*v] - [n*u] - [n*v], where u=sqrt(7), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*u + n*v] - [n*u] - [n*v] where u = sqrt(7) and v =
 *            1/u. Implemented by a(n) = floor((64*n*n)/7)^(1/2) -
 *            floor((7*n*n))^(1/2) - floor((n*n)/7)^(1/2)
 * Range:     u8 n <= 5, u16 n <= 84, u32 n <= 8945, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root: floor(sqrt(x)) using only integer arithmetic. */
static seq_t isqrt_seq(seq_t x) {
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0 || mid <= x / mid) {   /* mid*mid <= x ? use division to avoid overflow */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;

    /* floor(n*sqrt(7))      = floor( sqrt(7*n^2) )   */
    seq_t term1 = isqrt_seq(n2 * (seq_t)7);

    /* floor(n/sqrt(7))      = floor( sqrt(n^2/7) )   */
    seq_t term2 = isqrt_seq(n2 / (seq_t)7);

    /* floor(8n/sqrt(7))   = floor( sqrt((64*n^2)/7) ) */
    seq_t term3 = isqrt_seq((n2 << 6) / (seq_t)7);

    return term3 - term1 - term2;
}
