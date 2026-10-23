/*
 * A040805  Continued fraction for sqrt(834).
 * https://oeis.org/A040805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) are the partial quotients of the continued fraction
 *            expansion of sqrt(834). The sequence starts with a(0) = 28 and
 *            has period length 22. The coefficients for n ≥ 1 are taken from
 *            the periodic part (a(1)…a(22))
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const seq_t SQRT_834 = (seq_t)834;          /* D = 834 */
static const seq_t INT_SQRT_834 = (seq_t)28;       /* floor(sqrt(834)) = 28 */
static const seq_t PERIOD_TERMINATOR = INT_SQRT_834 * 2;  /* 56 */

static seq_t period_terms[64];  /* period length is 22, so 64 is safe */
static size_t period_len = 0;

/* Helper that computes the period once and caches it */
static void compute_period(void)
{
    if (period_len != 0) return;          /* already computed */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = INT_SQRT_834;

    /* Generate the periodic part starting from a(1) */
    do
    {
        /* m = d*a - m */
        m = d * a - m;
        /* d = (D - m*m) / d */
        d = (SQRT_834 - m * m) / d;
        /* a = (a0 + m) / d */
        a = (INT_SQRT_834 + m) / d;

        period_terms[period_len++] = a;
    } while (a != PERIOD_TERMINATOR);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* ensure period is computed */
    compute_period();

    if (n == 0) {
        return INT_SQRT_834;  /* a(0) = 28 */
    }

    /* For n ≥ 1, index into the periodic part */
    idx_t idx = (n - 1) % (idx_t)period_len;
    return period_terms[(size_t)idx];
}
