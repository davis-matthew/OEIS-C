/*
 * A206958  Expansion of f(x^5, -x^7) - x * f(-x, x^11) in powers of x where
 *          f() is Ramanujan's two-variable theta function.
 * https://oeis.org/A206958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k∈ℤ} (-1)^{⌊k/2⌋} ( [n = 6k^2 - k] - [n = 6k^2 + 5k +
 *            1] )
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline unsigned long long int_sqrt(unsigned long long v) {
    unsigned long long r = (unsigned long long)(sqrt((long double)v));
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

seq_t a(idx_t n) {
    unsigned long long NN = (unsigned long long)n;
    unsigned long long D  = 1ULL + 24ULL * NN;              /* 1 + 24*n */
    unsigned long long r  = int_sqrt(D);
    if (r * r != D) return 0; /* not a perfect square → coefficient 0 */

    seq_t result = 0;
    /* numerators for the four possible k-values */
    long long nums[4] = { (long long)(1 + r), (long long)(1 - r),
                         (long long)(-5 + r), (long long)(-5 - r) };

    for (int i = 0; i < 4; ++i) {
        long long num = nums[i];
        if (num % 12 != 0) continue;            /* not integral k */
        long long k = num / 12;                 /* exact integer */

        /* floor(k/2) with correct behaviour for negative k */
        long long f_k2 = (k >= 0) ? (k / 2) : ((k - 1) / 2);
        seq_t sign = ((f_k2 & 1) ? -1 : 1);      /* (-1)^{floor(k/2)} */

        /* Determine whether this k corresponds to the first or second exponent */
        if (num == (long long)(1 + r) || num == (long long)(1 - r)) {
            result += sign;   /* coefficient +sign for 6k^2 - k */
        } else {
            result -= sign;   /* coefficient -sign for 6k^2 + 5k + 1 */
        }
    }

    return result;
}
