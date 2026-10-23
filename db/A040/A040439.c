/*
 * A040439  Continued fraction for sqrt(461).
 * https://oeis.org/A040439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(461).
   a(0) = ⌊√461⌋ = 21.
   a(n) for n ≥ 1 are the periodic part of the continued‑fraction
   expansion of √461; the period ends with the term 2*a(0) = 42. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

static bool initialized = false;
static seq_t period[20];     /* period length is 15 for D = 461  */
static size_t period_len = 0;
static seq_t a0_val = 0;

/* Compute the period of the continued fraction for √461 once. */
static void init_period(void)
{
    const idx_t D = 461;
    /* Integer part of √D */
    idx_t a0 = 0;
    for (a0 = 0; (a0 + 1) * (a0 + 1) <= D; ++a0)
        ;
    a0_val = (seq_t)a0;

    /* Continued‑fraction computation (classical algorithm). */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    period_len = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = (seq_t)a;
    } while (a != 2 * a0);   /* last term equals 2*a0 */

    initialized = true;
}

/* OEIS entry: a(n) = term n of the continued fraction of √461. */
seq_t a(idx_t n)
{
    if (!initialized)
        init_period();

    if (n == 0)
        return a0_val;

    /* n ≥ 1 → use periodic part. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
