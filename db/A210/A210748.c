/*
 * A210748  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210747; see the Formula section.
 * https://oeis.org/A210748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(k-1) in v(r,x), where r is the
 *            smallest integer with r(r+1)/2 ≥ n and k = n – r(r-1)/2. The
 *            polynomials satisfy u(1,x)=v(1,x)=1 u(n,x)=2x u(n-1,x)+(x+1)
 *            v(n-1,x)+1 v(n,x)=(x+1) u(n-1,x)+(x+1) v(n-1,x)+1 . The sequence
 *            enumerates the coefficients of v(r,x) in degree o...
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined index – safe fallback */

    /* --- find the row r such that r(r+1)/2 ≥ n --- */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1;                  /* grow bound until it is large enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* position inside the row (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;   /* T(r‑1) */
    idx_t pos = n - prev_tri;           /* 1 ≤ pos ≤ r */

    /* --- compute v(r,x) iteratively --- */
    seq_t *u_prev = NULL;
    seq_t *v_prev = NULL;

    /* row 1 */
    u_prev = (seq_t*)malloc(sizeof(seq_t));
    v_prev = (seq_t*)malloc(sizeof(seq_t));
    if (!u_prev || !v_prev) abort();
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    if (r == 1) {                      /* trivial case */
        seq_t result = v_prev[0];
        free(u_prev);
        free(v_prev);
        return result;
    }

    for (idx_t cur = 2; cur <= r; ++cur) {
        size_t new_len = (size_t)cur;
        seq_t *u_new = (seq_t*)malloc(sizeof(seq_t) * new_len);
        seq_t *v_new = (seq_t*)malloc(sizeof(seq_t) * new_len);
        if (!u_new || !v_new) abort();

        /* constant term (k = 0) */
        u_new[0] = v_prev[0] + (seq_t)1;
        v_new[0] = u_prev[0] + v_prev[0] + (seq_t)1;

        /* coefficients for k ≥ 1 */
        for (size_t k = 1; k < new_len; ++k) {
            seq_t upk1   = u_prev[k - 1];
            seq_t vpkr1  = v_prev[k - 1];
            seq_t upk    = (k < cur - 1) ? u_prev[k] : (seq_t)0;
            seq_t vpkr   = (k < cur - 1) ? v_prev[k] : (seq_t)0;
            u_new[k] = (seq_t)2 * upk1 + vpkr + vpkr1;
            v_new[k] = upk + upk1 + vpkr + vpkr1;
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
    }

    seq_t result = v_prev[pos - 1];
    free(u_prev);
    free(v_prev);
    return result;
}
