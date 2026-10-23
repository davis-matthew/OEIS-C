/*
 * A040859  Continued fraction for sqrt(889).
 * https://oeis.org/A040859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(889) (OEIS A040859).
   The sequence is periodic after the initial term a(0)=29.
   The period ends when a term equals 2·a0. */
#include <stdbool.h>
#include <stddef.h>

/* maximum period length for sqrt(889) – less than 60 for sure */
#define MAX_PERIOD 64

static bool init_done = false;
static seq_t period[MAX_PERIOD];
static size_t period_len = 0;
static const uint64_t a0_val = 29;            /* floor(sqrt(889)) */

/* Build the period of the continued fraction once */
static void init_period(void)
{
    if (init_done) return;

    const uint64_t D = 889ULL;          /* D = 889 */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0_val;

    size_t len = 0;
    while (1) {
        /* standard recurrence for continued fraction of sqrt(D) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0_val + m) / d;

        period[len++] = (seq_t)a;          /* store term */
        if (a == 2 * a0_val) break;        /* end of period */
        if (len >= MAX_PERIOD) break;      /* safety check */
    }
    period_len = len;
    init_done = true;
}

seq_t a(idx_t n)
{
    init_period();
    if (n == 0) return (seq_t)a0_val;
    /* a(1) corresponds to period[0] */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
