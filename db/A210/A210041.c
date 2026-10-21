/*
 * A210041  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209758; see the Formula section.
 * https://oeis.org/A210041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened row‑major list of the coefficients of
 *            u(n,x) * where the polynomials u and v satisfy the recurrence *
 *            * u(1,x) = 1, v(1,x) = 1 * u(n,x) = u(n‑1,x) + v(n‑1,x) + 1 *
 *            v(n,x) = x*u(n‑1,x) + 2x*v(n‑1,x) + 1 (for n > 1) * * The
 *            sequence a(n) consists of the coefficients of u(n,x)...
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n‑th term of sequence A210041 (1‑based index). */
seq_t a(idx_t n)
{
    /* Handle the first term directly. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Find the row containing the n‑th term and its offset within that row. */
    idx_t terms_before = 1;   /* terms contributed by row 1 */
    idx_t row = 2;            /* start examining rows from 2 onward */

    while (terms_before + (row - 1) < n) {
        terms_before += (row - 1);
        ++row;
    }

    /* Offset within the found row (0‑based). */
    idx_t offset = n - terms_before - 1;

    /* Prepare arrays for u and v coefficients of the previous row. */
    size_t len_u_prev = 1;                 /* u(1,x) has one coefficient */
    size_t len_v_prev = 1;                 /* v(1,x) has one coefficient */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t) * len_u_prev);
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t) * len_v_prev);
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Iterate to build rows up to the required one. */
    for (idx_t cur = 2; cur <= row; ++cur) {
        size_t len_u_cur = (cur == 1) ? 1 : (size_t)(cur - 1);
        size_t len_v_cur = (size_t)cur;

        seq_t *u_cur = (seq_t *)malloc(sizeof(seq_t) * len_u_cur);
        seq_t *v_cur = (seq_t *)malloc(sizeof(seq_t) * len_v_cur);

        /* Initialize new v with the constant term 1 and zeros elsewhere. */
        for (size_t k = 0; k < len_v_cur; ++k) {
            v_cur[k] = (seq_t)0;
        }
        v_cur[0] += (seq_t)1;  /* constant term from recurrence */

        /* Add shifted contributions of u_prev and v_prev to v_cur. */
        for (size_t i = 0; i < len_u_prev; ++i) {
            v_cur[i + 1] += u_prev[i];  /* x * u_prev[i] */
        }
        for (size_t i = 0; i < len_v_prev; ++i) {
            v_cur[i + 1] += (seq_t)2 * v_prev[i];  /* 2x * v_prev[i] */
        }

        /* Initialize new u with zeros. */
        for (size_t k = 0; k < len_u_cur; ++k) {
            u_cur[k] = (seq_t)0;
        }

        /* Add contributions of u_prev and v_prev to u_cur. */
        for (size_t i = 0; i < len_u_prev; ++i) {
            u_cur[i] += u_prev[i];   /* u(prev) */
        }
        for (size_t i = 0; i < len_v_prev; ++i) {
            u_cur[i] += v_prev[i];   /* v(prev) */
        }
        u_cur[0] += (seq_t)1;  /* constant term from recurrence */

        /* Free memory held by the previous row. */
        free(u_prev);
        free(v_prev);

        /* Move current arrays to previous for the next iteration. */
        u_prev   = u_cur;
        v_prev   = v_cur;
        len_u_prev = len_u_cur;
        len_v_prev = len_v_cur;
    }

    /* Retrieve the coefficient at the required offset. */
    seq_t result = u_prev[offset];

    /* Clean up. */
    free(u_prev);
    free(v_prev);

    return result;
}
