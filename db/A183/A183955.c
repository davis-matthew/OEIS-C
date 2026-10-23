/*
 * A183955  Number of strings of numbers x(i=1..4) in 0..n with sum i^2*x(i)
 *          equal to n*16.
 * https://oeis.org/A183955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count solutions to x1 + 4*x2 + 9*x3 + 16*x4 = 16*n with 0 ≤ xi ≤
 *            n. This is the definition of OEIS sequence A183955
 * Range:     u8 n <= 14, u16 n <= 100, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 for valid queries; for n==0 return 0 (unused). */
    if (n == 0) return 0;

    seq_t total = 0;

    /* Loop over possible values of x4. */
    for (idx_t x4 = 0; x4 <= n; ++x4) {
        /* Remaining sum after fixing x4: base = 16*(n - x4). */
        int64_t base = (int64_t)16 * (int64_t)(n - x4);

        /* Maximum x3 that can still satisfy the equation: floor(base/9), but not above n. */
        idx_t max_x3 = (base >= 0) ? (idx_t)(base / 9) : 0;
        if (max_x3 > n) max_x3 = n;

        /* For each x3, count admissible pairs (x2, x1). */
        for (idx_t x3 = 0; x3 <= max_x3; ++x3) {
            int64_t S = base - (int64_t)9 * (int64_t)x3;   /* S = 16*(n-x4) - 9*x3 */

            /* Compute lower and upper bounds for x2. */
            int64_t L; /* lower bound for x2 */
            if (S > (int64_t)n) {
                /* ceil( (S - n) / 4 ) */
                L = (S - (int64_t)n + 3) / 4;
            } else {
                L = 0;
            }

            int64_t U = S / 4;   /* floor( S / 4 ) */
            if (U > (int64_t)n) U = (int64_t)n;

            if (L <= U) {
                total += (seq_t)(U - L + 1);
            }
        }
    }

    return total;
}
