/*
 * A040925  Continued fraction for sqrt(956).
 * https://oeis.org/A040925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This code computes the continued fraction expansion of √956.
 * a(0) = ⌊√956⌋ = 30.
 * For n ≥ 1 the terms are produced by the standard algorithm for square‑root
 * continued fractions.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Initial integer part of √956. */
static const seq_t a0_value = 30;

/* Cache of the periodic part of the expansion. */
#define MAX_PERIOD 128
static seq_t period_cache[MAX_PERIOD];
static size_t period_len = 0;
static bool initialized = false;

static void init_period(void)
{
    if (initialized) return;

    /* All variables use seq_t to support arbitrary‑precision builds. */
    const seq_t D = (seq_t)956;      /* the radicand */
    seq_t m = 0;                     /* m0 */
    seq_t d = 1;                     /* d0 */
    seq_t a = a0_value;              /* a0 */
    seq_t double_a0 = a0_value + a0_value; /* 2*a0 */

    for (;;)
    {
        m = d * a - m;                          /* m_{k+1} = d_k * a_k - m_k */
        d = (D - (m * m)) / d;                  /* d_{k+1} = (D - m_{k+1}²) / d_k */
        a = (a0_value + m) / d;                 /* a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋ */

        period_cache[period_len++] = a;         /* store a_{k+1} */

        if (a == double_a0)                     /* period finished */
            break;
        /* Safety guard for pathological cases. */
        if (period_len == MAX_PERIOD)
            break;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return a0_value;

    init_period();

    /* Terms 1, 2, 3, … correspond to the periodic part in order. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period_cache[idx];
}
