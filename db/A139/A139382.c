/*
 * A139382  Triangle read by rows, T(n,k) = (2^k-1) * T(n-1,k) + T(n-1,k-1).
 * https://oeis.org/A139382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r and c satisfy * r*(r+1)/2 >= n and n -
 *            r*(r-1)/2 = c * T(n,k) = (2^k-1)*T(n-1,k) + T(n-1,k-1) with
 *            T(1,1)=1
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 69, u64 n <= 126,
 *            u128 n <= 262, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define MAX_R 2000  /* sufficient for n up to about 2 000 000 */

static seq_t row_prev[MAX_R];
static seq_t row_cur [MAX_R];

/* Compute a(n) from the recurrence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range */

    /* find row r that contains index n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* index of the desired element inside that row (1‑based) */
    idx_t before = (r - 1) * r / 2;
    idx_t offset = n - before;            /* 1 <= offset <= r */

    /* initialise row 1 */
    row_prev[0] = 1;                      /* T(1,1) = 1 */

    /* build rows 2 … r */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t power = 2;           /* 2^1 for k=1 */
        for (idx_t j = 0; j < i; ++j) {   /* j corresponds to column k=j+1 */
            seq_t term = 0;

            /* contribution from T(i‑1,k) */
            if (j < i - 1) {
                term += row_prev[j] * (power - 1);
            }

            /* contribution from T(i‑1,k‑1) */
            if (j > 0) {
                term += row_prev[j - 1];
            }

            row_cur[j] = term;
            power <<= 1;           /* advance to 2^(k+1) for next j */
        }

        /* copy new row into row_prev for next iteration */
        for (idx_t j = 0; j < i; ++j) row_prev[j] = row_cur[j];
    }

    /* return the requested entry */
    return row_prev[offset - 1];
}
