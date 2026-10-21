/*
 * A055645  Integers in base 16 with each hexadecimal digit represented by 2
 *          decimal digits.
 * https://oeis.org/A055645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A055645 – Integers in base 16 with each hexadecimal digit represented
    by 2 decimal digits.
    Algorithm:  For each hexadecimal digit of n (from most significant to
    least), append its two‑digit decimal representation to a decimal
    accumulator.  Equivalent to: for each hex digit d, result = result*100 + d.
    Return the accumulated decimal number.  n = 0 returns 0.  */
seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0) return 0;

    /* Find the highest power of 16 not exceeding n */
    idx_t power = 1;
    while (power <= n / 16)  /* power * 16 <= n after multiplication */
        power *= 16;

    seq_t result = 0;
    idx_t remaining = n;

    /* Process digits from most significant to least significant */
    while (power > 0) {
        idx_t digit = remaining / power;            /* current hex digit (0..15) */
        remaining = remaining - digit * power;      /* subtract processed part */
        result = result * 100 + (seq_t)digit;       /* append two decimal digits */
        power /= 16;                                /* move to next lower digit */
    }

    return result;
}
