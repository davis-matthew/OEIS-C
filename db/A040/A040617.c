/*
 * A040617  Continued fraction for sqrt(643).
 * https://oeis.org/A040617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction of sqrt(643); a(n) are the partial quotients
 *            of sqrt(643)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = floor(sqrt(643)) = 25, a(n>0) repeat the period starting from a(1). */

#include <stdbool.h>
#include <stdint.h>

static bool initialized = false;
static seq_t period[256];
static idx_t period_len = 0;

/* Compute the period of the continued fraction of sqrt(643).  */
/* The period ends when a == 2*a0 (the first time this happens). */
static void compute_period(void) {
    if (initialized) return;
    initialized = true;

    const uint64_t N   = 643ULL;   /* the number under the square root */
    const uint64_t a0  = 25ULL;    /* floor(sqrt(643)) */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    idx_t i   = 0;

    do {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        period[i++] = (seq_t)a;
    } while (a != 2 * a0);

    period_len = i;
}

/* Return the n-th term of A040617.  */
seq_t a(idx_t n) {
    compute_period();
    if (n == 0) return (seq_t)25;          /* the integer part of sqrt(643) */
    idx_t idx = (n - 1) % period_len;      /* periodic part starts at a(1) */
    return period[idx];
}
