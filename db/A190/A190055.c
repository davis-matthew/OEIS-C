/*
 * A190055  a(n) = n + [n*r/t] + [n*s/t]; r=2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor((3*n)/4)
 * Range:     u8 n <= 8, u16 n <= 442, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9434 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t floor_sqrt3_of_n(seq_t n)
{
    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)). */
    seq_t val = 3 * n * n;          /* 3*n*n  (no overflow for test n) */

    /* Binary search for the largest integer m with m*m <= val. */
    seq_t low  = 0;
    /* high can be safely set to n*2+1, because n*sqrt(3) < 2*n for all n>0. */
    seq_t high = (n << 1) + 1;
    seq_t ans  = 0;

    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;

        if (mid2 <= val) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;   /* floor(n*sqrt(3)) */
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sqrt_part = floor_sqrt3_of_n(N);
    seq_t frac_part = (3 * N) / 4;
    return N + sqrt_part + frac_part;
}
