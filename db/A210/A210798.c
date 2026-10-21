/*
 * A210798  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210797; see the Formula section.
 * https://oeis.org/A210798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A210798 is the flattened triangle of coefficients of the
 *            polynomials v(n,x) defined recursively: u(1,x)=1, v(1,x)=1,
 *            u(n,x)=u(n-1,x)+x*v(n-1,x),
 *            v(n,x)=(x+2)*u(n-1,x)+(x-1)*v(n-1,x)+1
 * Range:     u8 n <= 41, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the nth term of the flattened triangle. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guarded just in case */

    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Index within the row (0‑based). */
    idx_t before_row = (r - 1) * r / 2;          /* number of terms before row r */
    idx_t idx_in_row = n - before_row - 1;       /* 0‑based column index */

    /* Allocate arrays for u and v of the previous row. */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    u_prev[0] = 1;
    v_prev[0] = 1;

    for (idx_t k = 2; k <= r; ++k) {
        /* Create new arrays for the next row (length k). */
        seq_t *u_new = (seq_t *)calloc(k, sizeof(seq_t));
        seq_t *v_new = (seq_t *)calloc(k, sizeof(seq_t));

        for (idx_t i = 0; i < k; ++i) {
            /* u(k) = u(k-1) + x * v(k-1) */
            seq_t up   = (i < k - 1) ? u_prev[i] : 0;
            seq_t vp   = (i > 0)      ? v_prev[i - 1] : 0;
            u_new[i] = up + vp;

            /* v(k) = (x+2)*u(k-1) + (x-1)*v(k-1) + 1 */
            seq_t part1 = 0;
            if (i < k - 1) part1 += 2 * u_prev[i];
            if (i > 0)     part1 += u_prev[i - 1];

            seq_t part2 = 0;
            if (i < k - 1) part2 -= v_prev[i];
            if (i > 0)     part2 += v_prev[i - 1];

            v_new[i] = part1 + part2 + ((i == 0) ? 1 : 0);
        }

        /* Free previous row arrays and move to the new row. */
        free(u_prev);
        free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
    }

    /* Extract the requested coefficient. */
    seq_t result = v_prev[idx_in_row];

    free(u_prev);
    free(v_prev);
    return result;
}
