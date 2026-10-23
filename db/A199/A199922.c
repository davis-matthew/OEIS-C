/*
 * A199922  Table read by rows, T(0,0) = 1 and for n>0, 0<=k<=3^(n-1) T(n,k) =
 *          gcd(k,3^(n-1)).
 * https://oeis.org/A199922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the sequence obtained by reading the table * T(0,0)=1
 *            and, for n>0, 0≤k≤3^(n‑1), T(n,k)=gcd(k,3^(n‑1)) * row by row.
 *            The nth term of a(n) equals the table entry * corresponding to
 *            the unique (row, column) pair with * cumulative index n. Row 0
 *            has length 1, row n>0 has length * 3^(n‑1)+1
 * Range:     u8 n <= 369, u16 n <= 3288, u32 n <= 3288, u64 n <= 3288,
 *            u128 n <= 3288, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ensure we have access to the gcd utility. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t once; all other cumulative counts fit safely
     * into seq_t because n≤10000 (rows < 10). */
    seq_t sn = (seq_t)n;

    /* pow_minus1 stores 3^(r‑1); for r==0 this value is unused. */
    seq_t pow_minus1 = 1;          /* 3^0 */
    seq_t cumulative  = 0;         /* number of elements processed so far */

    for (idx_t r = 0; ; ++r) {
        seq_t row_len;
        if (r == 0) {
            row_len = 1;          /* row 0 has a single element 1 */
        } else {
            row_len = pow_minus1 + 1;   /* 3^(r‑1)+1 */
        }

        seq_t next_cumulative = cumulative + row_len;

        if (sn < next_cumulative) {
            /* n lies within this row. */
            idx_t offset = (idx_t)(sn - cumulative);   /* column index k */

            if (r == 0) {
                return (seq_t)1;    /* T(0,0) = 1 */
            } else {
                seq_t k = (seq_t)offset;
                return gcd(k, pow_minus1);  /* T(r,k) = gcd(k,3^(r‑1)) */
            }
        }

        /* n is not in this row; advance to next row. */
        cumulative = next_cumulative;

        /* Update power of three for the next row. */
        if (r > 0) {
            pow_minus1 *= 3;  /* pow_minus1 becomes 3^(r) for next r+1 */
        }
        /* For r==0, pow_minus1 stays 1 (already 3^0). */
    }
}
