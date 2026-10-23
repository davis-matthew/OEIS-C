/*
 * A047205  Numbers that are congruent to {0, 3, 4} mod 5.
 * https://oeis.org/A047205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5·ceil(n/3) – 1 when n ≡ 0 (mod 3) * a(n) =
 *            5·ceil((n+1)/3) – 2 when n ≡ 2 (mod 3) * a(n) = 5·ceil((n+2)/3)
 *            – 5 when n ≡ 1 (mod 3) * This implements the OEIS definition of
 *            numbers ≡ {0,3,4} (mod 5)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n=0 is unused */
        return 0;

    idx_t r = n % 3;

    /* map the residue to the appropriate formula */
    if (r == 0) {                    /* n = 3k  */
        idx_t k = n / 3;
        return (seq_t)k * (seq_t)5 - (seq_t)1;
    } else if (r == 1) {             /* n = 3k – 2 */
        idx_t k = (n + 2) / 3;
        return (seq_t)k * (seq_t)5 - (seq_t)5;
    } else {                         /* r == 2, n = 3k – 1 */
        idx_t k = (n + 1) / 3;
        return (seq_t)k * (seq_t)5 - (seq_t)2;
    }
}
