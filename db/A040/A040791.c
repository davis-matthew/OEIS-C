/*
 * A040791  Continued fraction for sqrt(820).
 * https://oeis.org/A040791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction coefficients for sqrt(820). The sequence is
   periodic after the first term:
      a(0) = 28,
      a(n) = period[(n-1) mod period_len] for n ≥ 1,
   where period = [1,1,1,2,1,10,1,2,1,1,1,56] has length 12. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t period_array[128];
static size_t period_len = 0;
static bool period_computed = false;

static void compute_period(void)
{
    const idx_t D = 820U;
    const idx_t a0 = 28U;          /* ⌊√D⌋ */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    size_t pos = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period_array[pos++] = (seq_t)a;
        if (a == 2 * a0)          /* period ends when a == 2·a0 */
            break;
    }
    period_len = pos;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;          /* first term */

    if (!period_computed) {
        compute_period();
        period_computed = true;
    }

    /* n ≥ 1:  use period with zero‑based index (n-1) mod period_len */
    idx_t idx = (n - 1) % period_len;
    return period_array[idx];
}
