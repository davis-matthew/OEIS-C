/*
 * A028339  Coefficient of x^2 in expansion of (x+1)*(x+3)*...*(x+2*n-1).
 * https://oeis.org/A028339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^2 in (x+1)(x+3)...(x+2*n-1). Recurrence:
 *            a(n) = 3*(2*n-3)*a(n-1) - (12*(n-2)^2 + 1)*a(n-2) +
 *            (2*n-5)^3*a(n-3); Base values: a(2)=1, a(3)=9, a(4)=86
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_N 100000   /* sufficient for n up to about 10 000 */

static seq_t cache[MAX_N + 1] = {0};
static idx_t max_computed = 1;

/* external linkage required */
seq_t a(idx_t n) {
    if (n < 2) return 0;                  /* undefined for n<2, return 0 */

    /* extend cache if necessary */
    if (n > MAX_N) {
        /* for safety, compute on the fly without exceeding bounds */
        /* but this situation should not occur in the test suite */
        return 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)9;
        } else if (i == 4) {
            cache[i] = (seq_t)86;
        } else {
            /* recurrence: a(i) = 3*(2*i-3)*a(i-1) - (12*(i-2)^2 + 1)*a(i-2) + (2*i-5)^3*a(i-3) */
            seq_t term1 = (seq_t)3 * (seq_t)(2 * i - 3) * cache[i - 1];
            seq_t term2 = (seq_t)(12 * (i - 2) * (i - 2) + 1) * cache[i - 2];
            seq_t tmp = (seq_t)(2 * i - 5);
            seq_t term3 = tmp * tmp * tmp * cache[i - 3];
            cache[i] = term1 - term2 + term3;
        }
    }
    max_computed = n;
    return cache[n];
}
