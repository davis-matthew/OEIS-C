/*
 * A048846  Least positive integer k for which 29^n divides k!.
 * https://oeis.org/A048846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that 29^n divides k!
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 985 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t exp_29_in_factorial(idx_t m)
{
    idx_t sum = 0;
    idx_t p   = 29;          /* 29^1 */
    while (p <= m) {
        sum += m / p;
        /* avoid overflow when multiplying by 29 */
        if (p > (idx_t)(~(unsigned long long)0) / 29)
            break;
        p *= 29;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (29^0 divides 1!) */
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t low = 1;
    idx_t high = 29 * n;     /* 29*n is always enough, since exp_29_in_factorial(29*n) >= n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (exp_29_in_factorial(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
