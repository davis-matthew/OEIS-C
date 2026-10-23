/*
 * A040765  Continued fraction for sqrt(794).
 * https://oeis.org/A040765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√794⌋ = 28; for n > 0 the terms repeat the periodic part
 *            of the continued fraction expansion of √794. The period is:
 *            5,1,1,1,1,1,1,1,1,5,56
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t a0;                /* floor(sqrt(794)) */
static seq_t period[200];
static size_t period_len;
static bool initialized = false;

/* Initialise the period once */
static void init(void)
{
    if (initialized)
        return;
    a0 = floor_sqrt((seq_t)794);          /* a0 = 28 */
    period_len = 0;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* generate the periodic part until the first appearance of
       2*a0, which signals the last term of the period. */
    do
    {
        m = d * a - m;
        d = ((seq_t)794 - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    } while (a != 2 * a0);

    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    if (n == 0)
        return a0;                      /* a(0) */

    /* periodic part starts at index 1 -> offset by 1 */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
