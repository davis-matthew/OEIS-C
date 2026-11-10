/*
 * A065438  Complement of A065039.
 * https://oeis.org/A065438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th nonnegative integer not of the form A065039(m) =
 *            Sum_{k>=0} floor(m/10^k); found by binary search on the count x
 *            - max{m : A065039(m) <= x}
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A065039_a(seq_t n)
{
    seq_t sum = 0;
    seq_t t = n;
    while (t > 0) {
        sum += t;
        t /= 10;
    }
    return sum;
}

/* largest m with A065039_a(m) <= x (A065039 is strictly increasing) */
static seq_t A065438_maxm(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (A065039_a(mid) <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    /* count of non-attained values in [0, x] is x - maxm(x) >= x/11 - 1 */
    seq_t lo = 0;
    seq_t hi = target * 12 + 30;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t cnt = mid - A065438_maxm(mid);
        if (cnt >= target) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
