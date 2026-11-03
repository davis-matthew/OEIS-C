/*
 * A078607  Least positive integer x such that 2*x^n > (x+1)^n.
 * https://oeis.org/A078607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(1/(2^(1/n)-1)) (n>1, a(0)=1, a(1)=2)
 * Range:     u8 n <= 177, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 88 terms through n = 9697 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static inline int _cmp_check(idx_t n, uint64_t x)
{
    /* Returns 1 if 2*x^n > (x+1)^n, 0 otherwise.
       The test is done in long double log domain to avoid huge integers. */
    long double ln2 = logl(2.0L);
    long double ln_1 = logl((long double)x);
    long double ln_2 = logl((long double)(x + 1));
    long double left  = ln2 + (long double)n * ln_1;
    long double right = (long double)n * ln_2;
    return left > right;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Initial approximation using n / log_e(2) ≈ 1.442695... * n */
    long double approx_ld = ((long double)n) / logl(2.0L);
    uint64_t approx   = (uint64_t)(approx_ld + 0.5L);  /* nearest integer */
    if (approx < 1) approx = 1;

    /* Ensure high bound satisfies the inequality */
    uint64_t low  = 1;
    uint64_t high = approx + 2;
    while (!_cmp_check(n, high))
        high = high * 2 + 2;  /* increase until condition holds */

    /* Binary search for the smallest x such that 2*x^n > (x+1)^n */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (_cmp_check(n, mid))
            high = mid;
        else
            low = mid;
    }
    return (seq_t)high;
}
