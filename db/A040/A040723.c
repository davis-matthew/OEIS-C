/*
 * A040723  Continued fraction for sqrt(751).
 * https://oeis.org/A040723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficient at position n in the
 *            expansion of sqrt(751) (a(0) = floor(sqrt(751)) )
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool init_done = false;
static seq_t period[512];
static size_t period_len;

/* Compute the periodic part of the continued fraction of sqrt(751).
   The period ends when the coefficient equals 2*a0. */
static void init_period(void) {
    const uint64_t D = 751ULL;      /* The number under the square root */
    const uint64_t a0 = 27ULL;      /* floor(sqrt(751))  (27^2=729 < 751)   */
    uint64_t m = 0ULL;
    uint64_t d = 1ULL;
    uint64_t a = a0;
    size_t k = 0ULL;

    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[k++] = (seq_t)a;
    } while (a != 2ULL * a0);     /* period terminates when a == 2*a0 */

    period_len = k;
    init_done = true;
}

seq_t a(idx_t n) {
    if (!init_done) init_period();

    if (n == 0ULL)
        return (seq_t)27ULL;

    /* We are in the repeating part: n-1 because a(1) is the first
       periodic coefficient. */
    size_t idx = (size_t)((n - 1ULL) % (idx_t)period_len);
    return period[idx];
}
