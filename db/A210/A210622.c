/*
 * A210622  Decimal expansion of 377/120.
 * https://oeis.org/A210622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A210622
   Decimals of 377/120:
   a(1)=3, a(2)=1, a(3)=4, a(4)=1, a(5)=6, a(n)=6 for n≥5
   The digits are obtained by repeatedly dividing the remainder
   of 377 by 120. */
seq_t a(idx_t n)
{
    const idx_t p = 377, q = 120;   /* numerator and denominator */

    if (n == 1)
        return (seq_t)(p / q);      /* integer part */

    idx_t r = p % q;                /* initial remainder */
    seq_t digit = 0;

    /* Compute successive decimal digits */
    for (idx_t i = 2; i <= n; ++i) {
        r = r * 10;
        digit = (seq_t)(r / q);
        r = r % q;
    }

    return digit;
}
