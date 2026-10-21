/*
 * A213611  Exact decimal expansion of the standard atmosphere in SI units.
 * https://oeis.org/A213611
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, cons, full, fini
 * Formula:   a(n) = (101325 / 10^(5-(n-6))) % 10 for 6 ≤ n ≤ 11; a(n) = 0
 *            otherwise
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined below offset, return 0. */
    if (n < 6) return 0;

    /* After the finite decimal expansion, return 0. */
    if (n > 11) return 0;

    /* Value whose decimal digits are extracted. */
    seq_t val = 101325;

    /* Index of the desired digit (0 for most significant). */
    idx_t i = n - 6;

    /* Compute 10^(5-i). */
    idx_t exp = 5 - i;
    seq_t pow10 = 1;
    while (exp > 0) {
        pow10 *= 10;
        exp--;
    }

    return (val / pow10) % 10;
}
