/*
 * A040631  Continued fraction for sqrt(657).
 * https://oeis.org/A040631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(657) */
#include <stddef.h>
static seq_t period[64];          /* maximum possible period length < 64 */
static size_t period_len = 0;
static int period_done = 0;

static void compute_period(void) {
    const seq_t D   = 657;       /* the number under the square root */
    const seq_t a0  = 25;        /* floor(sqrt(657)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    size_t i = 0;
    while (1) {
        /* next triple (m,d,a) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[i++] = a;
        if (a == 2 * a0)  /* the period ends with 2*a0 */
            break;
        /* In practice the period is very short (18 terms) */
        if (i >= sizeof(period)/sizeof(period[0]))
            break;       /* safe guard, should never trigger */
    }
    period_len = i;
    period_done = 1;
}

seq_t a(idx_t n) {
    /* compute the periodic part once */
    if (!period_done)
        compute_period();

    if (n == 0u)               /* a(0) = floor(sqrt(657)) */
        return (seq_t)25;

    idx_t k = (n - 1u) % (idx_t)period_len; /* periodic part */
    return period[(size_t)k];
}
