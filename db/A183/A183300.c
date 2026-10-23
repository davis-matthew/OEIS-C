/*
 * A183300  Positive integers not of the form 2n^2.
 * https://oeis.org/A183300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(n/2) + 1/4)
 * Range:     u8 n <= 243, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A183300, offset 1. */

/* Integer square‑root: floor(sqrt(x)) for 64‑bit unsigned integers. */
static idx_t isqrt_idx(idx_t x)
{
    /* For x < 2^64, the square root fits in 32 bits, so mid*mid never
       overflows an idx_t.  Binary search is fast and requires only
       integer arithmetic. */
    idx_t lo = 0;
    idx_t hi = x;
    /* hi can be at most 0xFFFFFFFFULL; clamp to avoid useless iterations */
    if (hi > 0xFFFFFFFFULL)
        hi = 0xFFFFFFFFULL;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi; /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1.  For n == 0 the behaviour is irrelevant. */
    if (n == 0)
        return 0;

    /* m = floor(sqrt(n/2)), using integer division for n/2 is
       equivalent because floor(sqrt(n/2)) = floor(sqrt(floor(n/2))) for n>=1. */
    idx_t half = n >> 1;          /* integer division by 2 */
    idx_t m = isqrt_idx(half);

    seq_t res = (seq_t)n + (seq_t)m;

    /* bound = (m+1)*(2*m+1); if n > bound, add one more to the result. */
    seq_t bound = (seq_t)(m + 1) * ((seq_t)(2 * m) + 1);
    if ((seq_t)n > bound)
        res += 1;

    return res;
}
