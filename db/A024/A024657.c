/*
 * A024657  n written in fractional base 10/2.
 * https://oeis.org/A024657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n % 10) + 10 * (convert_base5_to_even_digits(n / 10)) *
 *            where convert_base5_to_even_digits interprets k = n / 10 in
 *            base‑5 and * replaces each radix‑5 digit d (0 ≤ d ≤ 4) by 2*d,
 *            then interprets the * resulting decimal digits. * * Example: n =
 *            24 → k = 2 (base‑5 “2”), r = 4; a(24) = 4 + 1...
 * Range:     u8 n <= 79, u16 n <= 4499, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t  q = n / 10;                 /* higher part of the index   */
    seq_t  r = (seq_t)(n % 10);        /* unit digit of the value    */
    seq_t  value = r;                  /* start with the unit digit  */
    idx_t  pow10 = 10;                 /* 10^1, 10^2, … for positions */

    while (q > 0) {
        idx_t d = q % 5;               /* base‑5 digit (0…4)         */
        q /= 5;
        value += ((seq_t)d * 2u) * (seq_t)pow10;  /* 2*d in that decimal position */
        pow10 *= 10;                   /* move to the next power of 10  */
    }

    return value;
}
