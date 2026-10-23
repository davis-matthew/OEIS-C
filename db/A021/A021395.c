/*
 * A021395  Decimal expansion of 1/391.
 * https://oeis.org/A021395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of the decimal expansion of 1/391.
   a(n) = floor(10^(n+1) / 391) mod 10,
   i.e. the n-th digit after the decimal point of 1/391. */

/* The expansion is periodic with period 176.  The following
   table is generated once and then used for all subsequent calls. */

#include <stddef.h>          /* for size_t */

static seq_t digits[200];   /* room for the period (176) */
static idx_t period  = 0;   /* actual period length, set after init */
static int ready = 0;       /* flag: table initialised? */

/* Initialise the table of digits of 1/391.  This is called lazily
   the first time a() is invoked. */
static void init_digits(void)
{
    if (ready) return;
    ready = 1;

    idx_t r = 1;          /* initial remainder */
    idx_t i = 0;

    /* Compute decimal digits until the remainder repeats.
       The period is ≤ 390 (φ(391)), so 200 entries are enough. */
    do {
        seq_t d = (seq_t)((r * 10) / 391);   /* next digit */
        digits[i++] = d;
        r = (r * 10) % 391;                  /* new remainder */
    } while (r != 1 && i < 200);              /* stop before repeat */

    period = i;            /* length of the periodic pattern */
}

/* Return the n-th decimal digit of 1/391.  Index 0 is the first
   digit after the decimal point. */
seq_t a(idx_t n)
{
    if (!ready) init_digits();          /* lazily initialise */

    /* The digits repeat every 'period' places. */
    return digits[(size_t)(n % period)];
}
