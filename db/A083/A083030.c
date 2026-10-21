/*
 * A083030  Numbers that are congruent to {0, 4, 7} mod 12.
 * https://oeis.org/A083030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 12k – 5 a(3k-1) = 12k – 8 a(3k-2) = 12k – 12 (k ≥ 1)
 *            Thus, depending on n mod 3, the value can be computed directly
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t  r = n % 3U;      /* remainder mod 3 */
    idx_t  k;
    seq_t  val;

    if (r == 0) {               /* n = 3k */
        k   = n / 3;
        val = (seq_t)(12 * k) - 5;
    } else if (r == 1) {        /* n = 3k–2 */
        k   = (n + 2) / 3;
        val = (seq_t)(12 * k) - 12;
    } else {                    /* r == 2, n = 3k–1 */
        k   = (n + 1) / 3;
        val = (seq_t)(12 * k) - 8;
    }

    return val;
}
