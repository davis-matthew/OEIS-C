/*
 * A021453  Decimal expansion of 1/449.
 * https://oeis.org/A021453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the (n+1)-th decimal digit of 1/449
   (offset 0: the first digit after the decimal point). */
seq_t a(idx_t n) {
    const idx_t M = 449;        /* modulus for long division */
    idx_t r = 1;                /* remainder before first digit */
    seq_t digit = 0;            /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;                     /* shift in next decimal place */
        digit = (seq_t)(r / M);         /* integer part is the digit */
        r = r % M;                      /* new remainder for next digit */
    }
    return digit;
}
