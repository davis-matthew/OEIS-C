/*
 * A021865  Decimal expansion of 1/861.
 * https://oeis.org/A021865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n‑th digit of the decimal expansion of 1/861
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period < 60, so only 60 digits need be stored. */

#include <stdint.h>
#include <stddef.h>

/* cached one‑period of digits */
static seq_t period[60];
static int period_initialised = 0;

/* compute the first 60 digits of 1/861 by long division */
static void init_period(void)
{
    if (period_initialised) return;

    seq_t remainder = (seq_t)1;          /* start with 1/861 */
    const seq_t denom   = (seq_t)861;
    const seq_t factor  = (seq_t)10;

    for (idx_t i = 0; i < 60; ++i) {
        seq_t r = remainder * factor;   /* multiply by 10 */
        period[i] = r / denom;          /* digit 0‑9 */
        remainder = r % denom;          /* new remainder */
    }
    period_initialised = 1;
}

/* public function requested by the specification */
seq_t a(idx_t n)
{
    init_period();
    idx_t m = n % 60;          /* period length */
    return period[m];
}
