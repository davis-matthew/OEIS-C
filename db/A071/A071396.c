/*
 * A071396  Rounded total surface area of a regular octahedron with edge
 *          length n.
 * https://oeis.org/A071396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(2*n^2*sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 46, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;

    /* raise high until high^2 > x */
    while (high * high <= x) {
        high = high * 2;
    }

    /* binary search for floor(sqrt(x)) */
    while (high - low > 1) {
        seq_t mid = (low + high) / 2;
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n^2 as seq_t */
    seq_t nsq = (seq_t)n * (seq_t)n;          /* n^2          */
    seq_t T   = (seq_t)2 * nsq;               /* T = 2*n^2    */

    /* z = 3*T^2  (equals (T*sqrt(3))^2) */
    seq_t z = ((seq_t)3 * T) * T;

    /* k = floor(sqrt(z)) = floor(T*sqrt(3)) */
    seq_t k = isqrt_seq(z);

    /* Decide whether to round up:
       round up if (3*T^2) ≥ (k+0.5)^2
       which is equivalent to 12*T^2 ≥ 4*k^2 + 4*k + 1   */
    seq_t lhs = ((seq_t)12 * T) * T;
    seq_t rhs = ((seq_t)4 * k) * k + ((seq_t)4 * k) + (seq_t)1;

    if (lhs >= rhs) {
        return k + (seq_t)1;
    } else {
        return k;
    }
}
