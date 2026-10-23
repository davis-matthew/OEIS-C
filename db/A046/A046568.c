/*
 * A046568  Numerators of the 1/4-Pascal triangle (by row).
 * https://oeis.org/A046568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046568: Numerators of the 1/4‑Pascal triangle (by row).
    Construction:
        row 1  :   1
        row 2  :   1   1
        row 3  :   1   1/4   1
        row n  (n ≥ 4):  each entry is the sum of the two entries above it.
    The sequence a(n) is the numerator of the fraction in row‑major order.
*/
#include <stddef.h>

typedef struct {
    seq_t  num;   /* numerator of the fraction  num / 2^exp  */
    unsigned exp; /* exponent of 2 in the denominator */
} Frac;

seq_t a(idx_t n)
{
    if (n <= 2u) return 1;          /* a(1) = a(2) = 1 */

    /* maximum row length needed for n ≤ 10000 is about 142.   */
    const size_t MAX_ROW = 200;
    Frac prev[MAX_ROW];
    Frac cur[MAX_ROW];

    /* initialise row 1 */
    prev[0].num  = (seq_t)1;
    prev[0].exp  = 0;
    size_t idx   = 1;                /* current index in the sequence */

    for (size_t row = 2; ; ++row) {
        size_t len = row;            /* number of entries in this row */
        for (size_t j = 0; j < len; ++j) {
            Frac val;

            if (row == 2) {                      /* row 2: all 1 */
                val.num = (seq_t)1;
                val.exp = 0;
            }
            else if (row == 3) {                 /* row 3: 1, 1/4, 1 */
                if (j == 0 || j == len - 1) {
                    val.num = (seq_t)1;
                    val.exp = 0;
                } else {
                    val.num = (seq_t)1;
                    val.exp = 2;                 /* 1/4  →  denominator 2^2 */
                }
            }
            else {                               /* row n ≥ 4 : sum of two above */
                if (j == 0 || j == len - 1) {
                    val.num = (seq_t)1;
                    val.exp = 0;
                }
                else {
                    Frac left  = prev[j-1];
                    Frac right = prev[j];

                    /* bring both fractions to the same exponent */
                    if (left.exp < right.exp) {
                        Frac tmp = left; left  = right; right = tmp;
                    }
                    unsigned shift = left.exp - right.exp;
                    seq_t tnum = (shift == 0) ? right.num
                                             : (right.num << shift);
                    seq_t sum  = left.num + tnum;
                    unsigned e = left.exp;

                    /* reduce by factors of 2 */
                    while ((sum & (seq_t)1) == 0 && e > 0) {
                        sum >>= 1;
                        --e;
                    }
                    val.num = sum;
                    val.exp = e;
                }
            }

            /* we are about to output this term */
            ++idx;
            if (idx == n) return val.num;

            cur[j] = val;       /* store for next row */
        }

        /* move current row to prev for next iteration */
        for (size_t j = 0; j < len; ++j) prev[j] = cur[j];
    }
}
