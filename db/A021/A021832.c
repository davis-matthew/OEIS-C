/*
 * A021832  Decimal expansion of 1/828.
 * https://oeis.org/A021832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point of 1/828, with n=0
 *            yielding the first digit after the point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t period_digits[200];
static idx_t period_start = 0;   /* index where the repeating cycle begins */
static idx_t period_len   = 0;   /* length of the repeating cycle        */
static unsigned initialized = 0; /* flag indicating that period is ready */

static void compute_period(void)
{
    /* Map each possible remainder (0 .. 827) to the index of the
       digit that will be produced from that remainder.  The value
       IDX_T_MAX is used as a sentinel for "not seen yet". */
    const idx_t NOT_SEEN = (idx_t)-1;
    idx_t seen[828];
    for (int i = 0; i < 828; ++i)
        seen[i] = NOT_SEEN;

    idx_t rem   = 1;   /* starting remainder for 1 / 828 */
    idx_t idx   = 0;   /* current digit index            */

    /* Generate digits until a remainder repeats.                     */
    for (;; ++idx) {
        if (seen[rem] != NOT_SEEN) {
            /* Cycle detected.  The cycle starts at seen[rem]               */
            period_start = seen[rem];
            period_len   = idx - period_start;
            break;
        }
        seen[rem] = idx;                           /* remember position */
        period_digits[idx] = (seq_t)((rem * 10) / 828);
        rem = (rem * 10) % 828;                   /* next remainder   */
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized)
        compute_period();

    if (n < period_start) {
        return period_digits[n];
    } else {
        idx_t cyc = (n - period_start) % period_len;
        return period_digits[period_start + cyc];
    }
}
