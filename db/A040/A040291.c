/*
 * A040291  Continued fraction for sqrt(309).
 * https://oeis.org/A040291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(309)
   a(0) = floor(sqrt(309)); all other terms repeat
   with the standard continued‑fraction period, ending when the
   coefficient equals 2*a(0). */
#include <stddef.h>
#include <stdint.h>

static int initialized = 0;
static seq_t a0;
static seq_t period[128];
static size_t period_len;

static void init(void) {
    if (initialized) return;

    const seq_t D = 309;

    /* compute a0 = floor(sqrt(D)) */
    a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) {
        a0 = a0 + 1;
    }

    /* continued fraction loop */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    size_t idx = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[idx++] = a;
    } while (a != a0 * 2);

    period_len = idx;
    initialized = 1;
}

seq_t a(idx_t n) {
    if (n == 0) {
        if (!initialized) init();
        return a0;
    }
    if (!initialized) init();
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
