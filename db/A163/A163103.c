/*
 * A163103  Decimal expansion of the astronomical unit (measured in meters).
 * https://oeis.org/A163103
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  cons, fini, full, nonn
 * Formula:   a(n) = the (n-12)-th decimal digit of 149597870700 for 12 ≤ n ≤
 *            23; a(n) = 0 for n < 12 or n > 23
 * Range:     u8 none, u16 none, u32 none, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The astronomical unit in meters is exactly 149 597 870 700, so this
 * sequence simply yields its decimal digits, followed by zeros. */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the defined offset or above the number
     * of digits (12).  The sequence begins at n = 12. */
    if (n < 12)
        return (seq_t)0;
    if (n > 23)
        return (seq_t)0;

    /* Value of the astronomical unit in meters.  It fits in any seq_t
     * chosen by the build that can hold this integer. */
    const seq_t value = (seq_t)149597870700ULL;

    /* Relative position within the 12-digit number.  r = 0 for the
     * first digit (n = 12) up to r = 11 for the last digit (n = 23). */
    const idx_t r = n - 12;          /* 0 ≤ r ≤ 11 */

    /* Exponent to shift the wanted digit to the units place: e = 11 – r. */
    const idx_t e = 11 - r;          /* 0 ≤ e ≤ 11 */

    /* Compute 10^e.  e is at most 11, so a small loop is fine and
     * does not incur any significant overhead. */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        pow10 *= (seq_t)10;

    /* Extract the digit: number / 10^e gives the integer whose last
     * digit is the required one.  Modulo 10 isolates that digit. */
    const seq_t digit = (value / pow10) % (seq_t)10;

    return digit;
}
