/*
 * A021842  Decimal expansion of 1/838.
 * https://oeis.org/A021842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (10^(n+1) / 838) mod 10 ) * (digits of the decimal
 *            expansion of 1/838)
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the fact that the decimal period of 1/838 is 418.
 * The sequence is periodic with that length, so we reduce n modulo the
 * period before generating the digit.  The remainder is updated by long
 * division:  r := r*10;  digit := r/838;  r := r%838.  All calculations
 * are performed in the smallest integer type that suffices, using only
 * the operators allowed by the problem statement. */

seq_t a(idx_t n)
{
    /* Period of 1/838: remove factors 2 and 5, then find the order of 10 mod 419. */
    static idx_t period = 0;
    if (!period) {
        /* Compute period = smallest t > 0 such that 10^t ≡ 1 (mod 419). */
        seq_t rem = 10 % 419;          /* 10^1 mod 419 */
        idx_t t = 1;
        while (rem != 1) {
            rem = (rem * 10) % 419;   /* next power of 10 modulo 419 */
            ++t;
        }
        period = t;                    /* period = 418 */
    }

    idx_t k = n % period;              /* use only the repeating block */

    seq_t remainder = 1;  /* initial remainder before first division */
    seq_t digit = 0;

    for (idx_t i = 0; i <= k; ++i) {
        remainder = remainder * 10;
        digit = remainder / 838;
        remainder = remainder % 838;
    }
    return digit;
}
