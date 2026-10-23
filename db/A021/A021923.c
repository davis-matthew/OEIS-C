/*
 * A021923  Decimal expansion of 1/919.
 * https://oeis.org/A021923
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

/* decimal digit of 1/919 after the decimal point */
static seq_t digits[919];
static idx_t period = 0;

/* compute the period and the first cycle of digits once */
static void init_digits(void)
{
    if (period != 0) return;

    idx_t remainder = 1;               /* starting remainder */
    for (idx_t i = 0; ; ++i) {
        remainder *= 10;                  /* shift left */
        seq_t digit = remainder / 919;    /* next decimal digit */
        remainder %= 919;                 /* new remainder */
        digits[i] = digit;
        if (remainder == 1) {             /* cycle completed */
            period = i + 1;               /* period length */
            break;
        }
        /* safety: for a prime <= 919, period < 919, so no overflow */
    }
}

seq_t a(idx_t n)
{
    init_digits();                       /* lazy initialization */
    idx_t idx = n % period;              /* index inside the period */
    return digits[idx];
}
