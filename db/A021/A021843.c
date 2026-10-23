/*
 * A021843  Decimal expansion of 1/839.
 * https://oeis.org/A021843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n‑th digit after the decimal point in the decimal
 *            expansion of 1/839, with a(0) being the first digit
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *digits = NULL;   /* array containing one full period of digits */
static idx_t period = 0;       /* period length of the expansion */

static void init_digits(void)
{
    if (digits != NULL)          /* already initialized */
        return;

    const idx_t modulus = 839;   /* denominator */
    int seen[modulus];           /* marks first occurrence of a remainder */

    for (idx_t i = 0; i < modulus; ++i)
        seen[i] = -1;

    seq_t *buf = (seq_t *)malloc(modulus * sizeof(seq_t));
    if (buf == NULL)
        return;                  /* allocation failure; all future calls return 0 */

    idx_t r = 1;                 /* initial remainder */
    idx_t pos = 0;

    /* Generate digits until a remainder repeats */
    while (seen[r] == -1) {
        seen[r] = (int)pos;
        seq_t digit = (seq_t)((r * 10u) / modulus);
        buf[pos] = digit;
        r = (r * 10u) % modulus;
        ++pos;
    }

    period = pos;
    digits = buf;
}

seq_t a(idx_t n)
{
    init_digits();               /* ensure digits array is ready */

    if (digits == NULL || period == 0)
        return (seq_t)0;         /* graceful fallback */

    idx_t idx = n % period;      /* position within the repeating block */
    return digits[idx];
}
