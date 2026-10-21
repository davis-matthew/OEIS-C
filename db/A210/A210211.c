/*
 * A210211  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210212; see the Formula section.
 * https://oeis.org/A210211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x): Let u(1,x)=1,
 *            v(1,x)=1. For n>1: u(n,x)=x*u(n-1,x)+v(n-1,x)+1;
 *            v(n,x)=u(n-1,x)+2x*v(n-1,x)+1. a(n) is the flattened triangle of
 *            coefficients of u(n,x), with constant term first. This function
 *            computes a(n) by iterating the recurrence for the require...
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

static idx_t
find_row(idx_t n) {
    /* return smallest r with r*(r+1)/2 >= n   */
    if (n == 0) return 0;
    idx_t low = 1;
    idx_t high = 1;
    /* increase high until it is enough (but not too large) */
    while ((__uint128_t)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t s = (__uint128_t)mid * (mid + 1) / 2;
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* determine the row containing index n */
    idx_t r = find_row(n);
    /* sum of elements before row r */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t offset = n - prev_sum - 1;   /* 0‑based column within row */

    /* special case: row 1 */
    if (r == 1) {
        return (seq_t)1;
    }

    /* allocate arrays for the previous row */
    seq_t *u_prev = (seq_t *)malloc(1 * sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc(1 * sizeof(seq_t));
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* iterate rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t len_prev = row - 1;
        idx_t len_cur  = row;

        seq_t *u_cur = (seq_t *)malloc(len_cur * sizeof(seq_t));
        seq_t *v_cur = (seq_t *)malloc(len_cur * sizeof(seq_t));

        /* constant term */
        u_cur[0] = v_prev[0] + (seq_t)1;
        v_cur[0] = u_prev[0] + (seq_t)1;

        /* higher degree terms */
        const seq_t two = (seq_t)2;
        for (idx_t k = 1; k < len_cur; ++k) {
            seq_t uprev_shift = u_prev[k - 1];                     /* u(n-1, k-1) */
            seq_t vprev_current = (k < len_prev) ? v_prev[k] : (seq_t)0;
            seq_t uprev_current = (k < len_prev) ? u_prev[k] : (seq_t)0;
            seq_t vprev_shift   = v_prev[k - 1];                   /* v(n-1, k-1) */

            u_cur[k] = uprev_shift + vprev_current;
            v_cur[k] = uprev_current + two * vprev_shift;
        }

        /* free previous row and advance */
        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }

    /* u_prev now contains row r */
    seq_t result = u_prev[offset];
    free(u_prev);
    free(v_prev);
    return result;
}
