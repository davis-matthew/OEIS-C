/*
 * A064784  Difference between n-th triangular number t(n) and the largest
 *          square <= t(n).
 * https://oeis.org/A064784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = n*(n+1)/2 - floor(sqrt(n*(n+1)/2))^2
 * Range:     u8 n <= 22, u16 n <= 368, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer square root of x (floor of sqrt).  Works for any
   unsigned integral seq_t without overflow. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    seq_t best = 0;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        /* mid*mid <= x  <=>  mid <= x/mid  (mid != 0 case handled) */
        if (mid == 0 || mid <= x / mid) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Triangular number t(n) = n*(n+1)/2 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* floor(sqrt(tri)) */
    seq_t root = isqrt_seq(tri);

    /* Square of the floor root */
    seq_t sq = root * root;

    /* Difference */
    return tri - sq;
}
