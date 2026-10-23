/*
 * A021353  Decimal expansion of 1/349.
 * https://oeis.org/A021353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/349 (sequence A021353) */
seq_t a(idx_t n)
{
    seq_t r = 1;      /* remainder before the decimal point */
    seq_t digit = 0;  /* current digit being computed */

    /* For each position i (0‑based) from 0 to n inclusive compute the digit */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                    /* shift remainder left by one decimal place */
        digit = r / 349;            /* next digit of the decimal expansion */
        r %= 349;                   /* update remainder */
    }
    return digit; /* digit is in 0..9 and fits in seq_t */
}
