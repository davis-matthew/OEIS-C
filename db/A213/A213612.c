/*
 * A213612  Decimal expansion of the duration of the Julian year in SI
 *          seconds.
 * https://oeis.org/A213612
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, cons, full, fini
 * Formula:   a(n) = decimal digit of 31557600 at position n‑7 (the first
 *            digit occurs at n = 8)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has offset 8.  Values before this offset are
       irrelevant for the tests, so we simply return 0. */
    if (n < 8)
        return 0;

    static const seq_t digits[8] = {3, 1, 5, 5, 7, 6, 0, 0};
    idx_t pos = n - 8;          /* 0‑based index into digits */
    if (pos < 8)
        return digits[pos];

    /* After the last digit of the integer 31557600 the decimal
       expansion continues with zeros. */
    return 0;
}
