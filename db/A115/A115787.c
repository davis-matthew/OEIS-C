/*
 * A115787  Duplicate of A063438.
 * https://oeis.org/A115787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 4 if n >= 8 and n ≡ 1 (mod 7); otherwise a(n) = 3
 * Range:     i8 n <= 22, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned, so n-1 never underflows for n >= 1 */
    if (n >= 8 && ((n - 1) % 7) == 0)
        return 4;
    return 3;
}
