/*
 * A209556  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209555; see the Formula section.
 * https://oeis.org/A209556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the flattened triangle of *
 *            coefficients of the polynomial v(n,x) generated jointly with *
 *            u(n,x). The polynomials satisfy * * u(1,x) = 1, v(1,x) = 1 *
 *            u(n,x) = x*u(n-1,x) + v(n-1,x) * v(n,x) = u(n-1,x) + x*v(n-1,x)
 *            + 1 * * The triangle is read row by row (row n has n coe...
 * Range:     u8 n <= 59, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
    /* find the row r such that T_{r-1} < n <= T_r */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    /* index of the first element of this row (1‑based) */
    idx_t before = (r - 1) * r / 2;
    /* zero‑based column within the row */
    idx_t col = n - before - 1;

#if defined(__cplusplus)
    /* C++: use vector */
    std::vector<seq_t> u(r), v(r);
    u[0] = 1;
    v[0] = 1;
    for (size_t len = 2; len <= (size_t)r; ++len) {
        std::vector<seq_t> new_u(len), new_v(len);
        new_u[0] = v[0];
        new_v[0] = u[0] + (seq_t)1;
        for (size_t i = 1; i < len; ++i) {
            seq_t a = u[i - 1];
            seq_t b = (i < len - 1 ? v[i] : (seq_t)0);
            new_u[i] = a + b;

            seq_t c = (i < len - 1 ? u[i] : (seq_t)0);
            seq_t d = v[i - 1];
            new_v[i] = c + d;
        }
        u.swap(new_u);
        v.swap(new_v);
    }
    return v[(size_t)col];
#else
    /* C: use malloc/calloc (seq_t is a plain integer type) */
    seq_t *u = calloc((size_t)r, sizeof(seq_t));
    seq_t *v = calloc((size_t)r, sizeof(seq_t));
    if (!u || !v) {
        /* allocation failed; return 0 as a safe fallback */
        free(u); free(v);
        return (seq_t)0;
    }
    u[0] = 1;
    v[0] = 1;
    for (size_t len = 2; len <= (size_t)r; ++len) {
        seq_t *new_u = calloc((size_t)len, sizeof(seq_t));
        seq_t *new_v = calloc((size_t)len, sizeof(seq_t));
        if (!new_u || !new_v) {
            free(u); free(v); free(new_u); free(new_v);
            return (seq_t)0;
        }
        new_u[0] = v[0];
        new_v[0] = u[0] + (seq_t)1;
        for (size_t i = 1; i < len; ++i) {
            seq_t a = u[i - 1];
            seq_t b = (i < len - 1 ? v[i] : (seq_t)0);
            new_u[i] = a + b;

            seq_t c = (i < len - 1 ? u[i] : (seq_t)0);
            seq_t d = v[i - 1];
            new_v[i] = c + d;
        }
        free(u); free(v);
        u = new_u;
        v = new_v;
    }
    seq_t result = v[(size_t)col];
    free(u); free(v);
    return result;
#endif
}
