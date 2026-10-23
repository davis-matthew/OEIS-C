/*
 * A015352  Inverse of 1343rd cyclotomic polynomial.
 * https://oeis.org/A015352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 for 0 ≤ n mod 1343 ≤ 16, 0 for 17 ≤ n mod 1343 ≤ 78, -1
 *            for 79 ≤ n mod 1343 ≤ 95, 0 for 96 ≤ n mod 1343 ≤ 1342. The
 *            sequence is periodic with period 1343
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the residue of n modulo 1343 */
    const idx_t period = 1343u;
    const idx_t r = n % period;          /* r is 0 … 1342 */

    /* decide the value according to the residue */
    if (r <= 16u)           return 1;
    if (r <= 78u)           return 0;
    if (r <= 95u)           return -1;
    /* r >= 96 */
    return 0;
}
