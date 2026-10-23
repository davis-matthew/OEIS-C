/*
 * A015852  Inverse of 1843rd cyclotomic polynomial.
 * https://oeis.org/A015852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (floor(n/1843) + 1) - (floor((n-19)/1843) + 1) -
 *            (floor((n-97)/1843) + 1) + (floor((n-116)/1843) + 1) The floors
 *            are ordinary integer divisions on unsigned types; any term whose
 *            argument is negative (i.e. n < offset) is omitted
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* term 1: (n/1843 + 1)  */
    res += (seq_t)(n / 1843) + 1;

    /* term 2: -( (n-19)/1843 + 1 ), only if n ≥ 19 */
    if (n >= 19u) {
        res -= (seq_t)((n - 19) / 1843) + 1;
    }

    /* term 3: -( (n-97)/1843 + 1 ), only if n ≥ 97 */
    if (n >= 97u) {
        res -= (seq_t)((n - 97) / 1843) + 1;
    }

    /* term 4: + ( (n-116)/1843 + 1 ), only if n ≥ 116 */
    if (n >= 116u) {
        res += (seq_t)((n - 116) / 1843) + 1;
    }

    return res;
}
