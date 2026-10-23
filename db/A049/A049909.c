/*
 * A049909  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the smallest integer such that 2^p < n -
 *          1 <= 2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{i=1}^{n-1} a(i)) - a(2^ceil(log2(n-1))+2-n), with
 *            a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes computed values up to the maximum requested n. */

#include <stdlib.h>

static seq_t *a_vals = NULL;   /* stored a(i) values, 1-indexed            */
static seq_t *pref   = NULL;   /* pref[i] = sum_{j=1}^{i} a(j), pref[0]=0   */
static idx_t   cap   = 0;      /* current allocated capacity (>= requested n) */
static idx_t   max_computed = 0; /* largest n whose value is computed */
static int     initialized = 0;

/* Ensure capacity at least n+1 (we use indices up to n). */
static void ensure_capacity(idx_t n)
{
    if (cap >= n) return;
    idx_t newcap = (cap > 0) ? cap * 2 : 16;
    while (newcap < n) newcap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, sizeof(seq_t) * (newcap + 1));
    pref   = (seq_t *)realloc(pref,   sizeof(seq_t) * (newcap + 1));
    if (!initialized) {
        pref[0] = 0; /* pref[0] must be zero */
    }
    cap = newcap;
}

/* Compute a(n) for any n >= 1, using memoisation and prefix sums. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for 0, return 0 */

    /* Ensure at least n is allocated */
    ensure_capacity(n);

    /* Compute values up to n if needed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            a_vals[1] = (seq_t)1;
            pref[1] = a_vals[1];
        } else if (i == 2) {
            a_vals[2] = (seq_t)2;
            pref[2] = pref[1] + a_vals[2];
        } else if (i == 3) {
            a_vals[3] = (seq_t)3;
            pref[3] = pref[2] + a_vals[3];
        } else {
            /* Compute p = floor(log2(n-2)) */
            idx_t t = i - 2;            /* t >= 2 for i>=4 */
            unsigned p = 0;
            while (((idx_t)1 << p) <= t) p++;
            p--;                         /* now p = floor(log2(t)) */

            /* m = 2^(p+1) + 2 - i */
            idx_t pow2 = ((idx_t)1 << (p + 1));
            idx_t m = pow2 + 2 - i;  /* guaranteed 1 <= m < i */

            seq_t sum_prev = pref[i - 1];
            seq_t val = sum_prev - a_vals[m];
            a_vals[i] = val;
            pref[i] = sum_prev + val;
        }
    }
    max_computed = (max_computed < n) ? n : max_computed;
    return a_vals[n];
}
