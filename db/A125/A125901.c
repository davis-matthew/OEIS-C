/*
 * A125901  Triangle of the denominators of the almost-harmonic numbers: n-th
 *          term in m-th row is denominator of (sum{k=1 to m} 1/k) - 1/n,
 *          1<=n<=m.
 * https://oeis.org/A125901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabl
 * Formula:   a(n) is the denominator of (sum_{k=1}^m 1/k) - 1/c, where m is
 *            the row containing index n and c is the column (1 ≤ c ≤ m)
 * Range:     u8 n <= 12, u16 n <= 54, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *lcm_arr = NULL;    /* lcm_arr[m] = lcm(1..m)   */
static seq_t *num_arr = NULL;    /* num_arr[m] = numerator of H_m with denominator lcm_arr[m] */
static idx_t max_m_computed = 0;

static void ensure_computed(idx_t m)
{
    if (max_m_computed >= m) return;
    /* allocate enough space */
    if (!lcm_arr) {
        lcm_arr = (seq_t*)malloc(sizeof(seq_t) * (m + 1));
        num_arr = (seq_t*)malloc(sizeof(seq_t) * (m + 1));
        /* row 0 is unused */
        lcm_arr[1] = 1;   /* lcm(1) = 1 */
        num_arr[1] = 1;   /* H1 = 1/1 -> numerator 1 */
        max_m_computed = 1;
    } else if (max_m_computed < m) {
        lcm_arr = (seq_t*)realloc(lcm_arr, sizeof(seq_t) * (m + 1));
        num_arr = (seq_t*)realloc(num_arr, sizeof(seq_t) * (m + 1));
    }

    for (idx_t k = max_m_computed + 1; k <= m; ++k) {
        seq_t d_prev = lcm_arr[k-1];
        seq_t d_new = (d_prev / gcd(d_prev, (seq_t)k)) * (seq_t)k;   /* lcm of previous and k */
        lcm_arr[k] = d_new;
        /* numeratorH_k = numeratorH_{k-1} * (d_new/d_prev) + d_new/k */
        seq_t factor = d_new / d_prev;
        seq_t term = d_new / (seq_t)k;
        num_arr[k] = num_arr[k-1] * factor + term;
    }
    max_m_computed = m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 just in case */

    /* find row m such that m(m+1)/2 >= n */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n) ++m;
    idx_t first_in_row = (m * (m - 1)) / 2 + 1;
    idx_t c = n - first_in_row + 1;   /* column (1-based) */

    ensure_computed(m);

    seq_t d      = lcm_arr[m];
    seq_t numH   = num_arr[m];

    seq_t numerator = numH * (seq_t)c - d;   /* H_m*n - d  */
    seq_t denom     = d * (seq_t)c;          /* d*n       */

    seq_t g = gcd(denom, numerator);
    if (g == 0) return denom;   /* shouldn't happen except when numerator=0, g=denom */
    return denom / g;
}
