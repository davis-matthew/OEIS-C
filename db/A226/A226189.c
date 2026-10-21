/*
 * A226189  Least positive integer k such that 1 + 1/2 + ... + 1/k >= sqrt(n).
 * https://oeis.org/A226189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that H_k >= sqrt(n), * where H_k = 1 +
 *            1/2 + ... + 1/k (harmonic number)
 * Range:     u8 n <= 37, u16 n <= 136, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   harmonic_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/harmonic_exact.h"

static const long double GAMMA = 0.57721566490153286060651209L;

/* Approximate harmonic number for large k  (log l + gamma + 1/(2k) - 1/(12k^2)) */
static long double harmonic_approx(idx_t k) {
    if (k == 0) return 0.0L;
    long double fk = (long double)k;
    return logl(fk) + GAMMA + 1.0L/(2.0L*fk) - 1.0L/(12.0L*fk*fk);
}

/* Wrapper that chooses exact or approximate based on k size */
static long double harmonic(idx_t k) {
    if (k <= 100000) return harmonic_exact(k);
    return harmonic_approx(k);
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1) = 1 by definition */
    long double target = sqrtl((long double)n);

    idx_t lo = 1, hi = 1;
    /* Find an upper bound hi such that H_hi >= target */
    while (harmonic(hi) < target) {
        if (hi > ((idx_t)-1) / 2) {   /* prevent overflow */
            hi = (idx_t)-1;
            break;
        }
        hi <<= 1;
    }
    /* Binary search for minimal k */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (harmonic(mid) >= target) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return (seq_t)lo;
}
