/*
 * A208517  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208516; see the Formula section.
 * https://oeis.org/A208517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(n- (r-1)r/2 -1) in v(r,x) where r is
 *            the unique integer with r(r+1)/2 ≥ n (v(n,x) defined by
 *            u(n,x)=u(n-1,x)+x*v(n-1,x), v(n,x)=x*u(n-1,x)+2x*v(n-1,x)+1,
 *            with u(1,x)=v(1,x)=1)
 * Range:     u8 n <= 34, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static idx_t find_row(idx_t n)
{
    /* Find r such that r(r+1)/2 >= n */
    unsigned long long r = (unsigned long long)(
        (sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* Correct possible rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) --r;
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    /* Find the triangle row r and position p (0‑based) of the nth term */
    idx_t r = find_row(n);
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t start = (idx_t)(prev + 1);          /* first index of row r */
    idx_t p = n - start;                      /* 0‑based degree */

    /* Allocate coefficient arrays up to degree p */
    size_t k = (size_t)(p + 1);
    seq_t *u[2] = { (seq_t*)calloc(k, sizeof(seq_t)),
                    (seq_t*)calloc(k, sizeof(seq_t)) };
    seq_t *v[2] = { (seq_t*)calloc(k, sizeof(seq_t)),
                    (seq_t*)calloc(k, sizeof(seq_t)) };
    if (!u[0] || !u[1] || !v[0] || !v[1]) {
        /* Allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    }

    int cur = 0, nxt = 1;
    u[cur][0] = (seq_t)1;
    v[cur][0] = (seq_t)1;

    for (idx_t curn = 2; curn <= r; ++curn) {
        for (size_t d = 0; d < k; ++d) {
            seq_t uprev_d     = u[cur][d];
            seq_t vprev_d     = v[cur][d];
            seq_t uprev_d_1   = (d > 0 ? u[cur][d - 1] : (seq_t)0);
            seq_t vprev_d_1   = (d > 0 ? v[cur][d - 1] : (seq_t)0);

            /* u(new) = u(old) + x*v(old) */
            u[nxt][d] = uprev_d;
            if (d > 0) u[nxt][d] += vprev_d_1;

            /* v(new) = x*u(old) + 2x*v(old) + 1 */
            if (d == 0) {
                v[nxt][d] = (seq_t)1;
            } else {
                v[nxt][d] = uprev_d_1 + (seq_t)2 * vprev_d_1;
            }
        }
        /* Swap current and next rows */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = v[cur][p];

    free(u[0]); free(u[1]); free(v[0]); free(v[1]);
    return result;
}
