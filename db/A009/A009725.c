/*
 * A009725  Expansion of e.g.f.: tan(x)*(1+x).
 * https://oeis.org/A009725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A009725 : Expansion of e.g.f. tan(x)*(1+x)
    a(2k-1) = T_k          (tangent number)
    a(2k)   = 2k * T_k
    T_1 = 1
    T_n = Σ_{j=1}^{n-1} binom(2n-2, 2j-1) * T_j * T_{n-j}   (n ≥ 2)
*/
#include <stdlib.h>

static seq_t *T_arr = NULL;   /* array of tangent numbers, indexed by k (T_0 unused) */
static idx_t  T_cap = 0;      /* current maximum k stored (valid indices 0..T_cap) */

/* ensure that tangent numbers up to T_cap0 ≥ needed are stored in T_arr */
static void ensure_T(idx_t needed)
{
    if (needed <= T_cap) return;          /* already computed */

    idx_t new_cap = needed;
    seq_t *new_arr = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));

    /* copy previous values */
    if (T_arr) {
        for (idx_t i = 0; i <= T_cap; ++i)
            new_arr[i] = T_arr[i];
        free(T_arr);
    } else {
        /* first allocation: initialise index 0 to 0 */
        new_arr[0] = 0;
        T_cap = 0;
    }

    /* initialise new entries to 0 */
    for (idx_t i = T_cap + 1; i <= new_cap; ++i)
        new_arr[i] = 0;

    /* compute new tangent numbers from T_cap+1 up to needed */
    for (idx_t n = T_cap + 1; n <= needed; ++n) {
        if (n == 1) {                /* base case */
            new_arr[1] = 1;
        } else {
            seq_t sum = 0;

            /* binom = C(2n-2, 1) initially */
            seq_t binom = (seq_t)(2 * n - 2);
            idx_t m = 2 * n - 2;         /* fixed value for this n */

            for (idx_t j = 1; j <= n - 1; ++j) {
                /* term for this j: binom * T_j * T_{n-j} */
                seq_t part = new_arr[j] * new_arr[n - j];
                part = part * binom;
                sum += part;

                /* compute next binom for r = 2*(j+1)-1 if needed */
                if (j < n - 1) {
                    idx_t r = 2 * j - 1;        /* current odd r */
                    /* binom_new = binom * (m - r) * (m - (r+1)) / ((r+2)*(r+1)) */
                    seq_t tmp = binom * (seq_t)(m - r);
                    tmp = tmp * (seq_t)(m - (r + 1));
                    seq_t denom = (seq_t)((r + 2) * (r + 1));
                    binom = tmp / denom;
                }
            }
            new_arr[n] = sum;
        }
    }

    T_arr = new_arr;
    T_cap = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* k = ceil((n+1)/2) gives the index of the relevant tangent number */
    idx_t k = (n + 1) / 2;
    ensure_T(k);

    seq_t val = T_arr[k];
    if (n & 1) {                    /* n is odd: a_n = T_k */
        return val;
    } else {                        /* n is even: a_n = n * T_k */
        return val * (seq_t)n;
    }
}
