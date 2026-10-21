/*
 * A080524  Triangle read by rows in which the n-th row contains n distinct
 *          numbers whose sum is n^n. The numbers are terms of an arithmetic
 *          progression with a common difference 1 or 2 respectively
 *          accordingly as n is odd or even.
 * https://oeis.org/A080524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term obtained by reading the triangle of rows
 *            r = 1,2,3,... Each row r contains r terms that form an
 *            arithmetic progression with difference 1 if r is odd, 2 if r is
 *            even. The sum of the row is r^r. For odd r: a(n) = r^(r-1) -
 *            (r-1)/2 + (i-1) For even r: a(n) = r^(r-1) - (r-1) +...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* find the row r that contains term n */
    idx_t r = 1;
    idx_t cumulative = 1;   /* r*(r+1)/2 for current r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* index of the term within its row (1‑based) */
    idx_t i = n - (r * (r - 1)) / 2;  /* i = n - (r-1)*r/2 */

    /* compute r^(r-1) */
    seq_t power = 1;
    if (r > 1) {
        seq_t base = (seq_t)r;
        idx_t exp = r - 1;
        for (idx_t k = 0; k < exp; ++k)
            power *= base;
    }

    /* compute the term */
    seq_t term;
    if ((r & 1) == 1) { /* odd r, difference = 1 */
        seq_t sub   = (seq_t)((r - 1) / 2);
        seq_t add   = (seq_t)(i - 1);
        term = power - sub + add;
    } else {            /* even r, difference = 2 */
        seq_t sub   = (seq_t)(r - 1);
        seq_t add   = (seq_t)2 * (seq_t)(i - 1);
        term = power - sub + add;
    }

    return term;
}
