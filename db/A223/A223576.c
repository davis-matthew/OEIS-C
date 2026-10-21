/*
 * A223576  T(n,k)=Number of nXk 0..2 arrays with antidiagonals unimodal.
 * https://oeis.org/A223576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the numbers T(i,j) for i,j ≥ 1 in order of
 *            increasing i+j. For each total s = i+j, i runs from s‑1 down to
 *            1. The number T(i,j) is defined by T(i,j) =
 *            U(min(i,j))^(max(i,j)-min(i,j)+1) * ∏{ U(t)^2 , t = 1 ..
 *            min(i,j)-1 }, where U(z) = (z^4 + 6*z^3 + 23*z^2 + 18*z + 24) /
 *            24
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 84, u128 n <= 217,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t U_arr[200] = {0};   /* precomputed U(z) values */
static seq_t pref[200] = {0};    /* prefix products of squares of U(z) */
static idx_t Umax = 0;

/* Ensure that U_arr[1..up_to] and pref[0..up_to] are computed. */
static void ensure_U(idx_t up_to)
{
    if (up_to <= Umax) return;
    if (up_to >= sizeof(U_arr)/sizeof(U_arr[0])) up_to = sizeof(U_arr)/sizeof(U_arr[0]) - 1;

    if (Umax == 0) {
        pref[0] = 1;
        Umax = 1;
    }
    for (idx_t v = Umax; v <= up_to; ++v) {
        seq_t m = v;
        seq_t u = ((m*m*m*m + (seq_t)6*m*m*m + (seq_t)23*m*m + (seq_t)18*m + (seq_t)24) / 24);
        U_arr[v]  = u;
        pref[v] = pref[v-1] * u * u;   /* product of squares */
    }
    Umax = up_to;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find smallest s such that C(s) = s*(s-1)/2 >= n */
    idx_t s = 2;
    while ((seq_t)s*(s-1)/2 < n) ++s;

    idx_t prevC = (s-1)*(s-2)/2;          /* C(s-1) */
    idx_t offset = n - prevC - 1;         /* 0‑based position within this sum block */

    idx_t i = (s-1) - offset;             /* row */
    idx_t j = 1 + offset;                 /* column */

    idx_t m = (i < j) ? i : j;            /* min(i,j) */
    idx_t M = (i < j) ? j : i;            /* max(i,j) */

    seq_t exp = (seq_t)(M - m + 1);        /* exponent for U(m) */
    ensure_U(m);                          /* compute needed U values */

    seq_t base  = U_arr[m];
    seq_t powVal = big_pow(base, exp);
    seq_t prodSq = pref[m-1];              /* product of squares up to m-1 */

    return powVal * prodSq;
}
