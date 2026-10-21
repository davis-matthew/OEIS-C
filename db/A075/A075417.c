/*
 * A075417  Squares of A002282: a(n) = (8*(10^n - 1)/9)^2.
 * https://oeis.org/A075417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8*(10^n - 1)/9)^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                 = 64*(10^n - 1)^2/81           */
/*                                             */
/* Uses the definition directly; this avoids the            */
/* subtraction that would be required by the linear         */
/* recurrence, and works with unsigned seq_t in all builds. */

seq_t a(idx_t n)
{
    /* compute 10^n as a seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= 10;                 /* 10^n in the current type    */
    }

    /* (10^n - 1)/9 is exact because 10^n ≡ 1 (mod 9)              */
    seq_t x = (pow10 - 1) / 9;
    x *= 8;                          /* 8*(10^n - 1)/9               */

    return x * x;                     /* square the result             */
}
