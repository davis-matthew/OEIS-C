/*
 * A063823  Expansion of g.f. (1 - 2*x^2 - 3*x^3)/((1 - x^3)*(1 - 2*x)).
 * https://oeis.org/A063823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) for n ≥ 4 a(0)=1, a(1)=2,
 *            a(2)=2, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* pointers to memoised values and bookkeeping of allocation size */
static seq_t *a_vals = NULL;   /* a[n] values */
static size_t capacity = 0;    /* allocated capacity in elements  */
static size_t len      = 0;    /* number of values currently computed (len = max_n+1) */

/* enlarge the storage so that indices < need are valid */
static void ensure_capacity(size_t need)
{
    if (need <= capacity) return;
    size_t newcap = capacity ? capacity << 1 : 4;
    while (newcap < need) newcap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, newcap * sizeof(seq_t));
    capacity = newcap;
}

/* sequence a(n) for arbitrary or 64‑bit integer types */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    ensure_capacity(target + 1);

    /* compute values up to target if not already done */
    if (len <= target) {
        for (size_t k = len; k <= target; ++k) {
            if (k == 0)
                a_vals[0] = (seq_t)1;
            else if (k == 1)
                a_vals[1] = (seq_t)2;
            else if (k == 2)
                a_vals[2] = (seq_t)2;
            else if (k == 3)
                a_vals[3] = (seq_t)2;
            else
                a_vals[k] = (seq_t)2 * a_vals[k-1] + a_vals[k-3] - (seq_t)2 * a_vals[k-4];
        }
        len = target + 1;
    }
    return a_vals[target];
}
