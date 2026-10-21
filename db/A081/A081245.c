/*
 * A081245  Number of days in months in the Haab year of Mayan/Mesoamerican
 *          calendars.
 * https://oeis.org/A081245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20 for n mod 19 ≠ 0; a(n) = 5 for n mod 19 = 0 (periodic
 *            Haab calendar months)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; compute modulo 19. */
    return (n % 19 == 0) ? (seq_t)5 : (seq_t)20;
}
