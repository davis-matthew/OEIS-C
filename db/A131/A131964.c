/*
 * A131964  Expansion of f(x^2, x^10) / f(x, x^3) in powers of x where f(, )
 *          is Ramanujan's general theta function.
 * https://oeis.org/A131964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A131964:  Expansion of f(x^2,x^10)/f(x,x^3).
 *  Recurrence:  a(0)=1,
 *                a(n) = (1/n) * Σ_{k=1..n} (Σ_{d|k} d*c_d) * a(n-k),
 *  where c_d is the period‑24 sequence
 *      [1,0,1,-2,1,-1,1,-1,1,0,1,-2,1,0,1,-1,1,-1,1,-2,1,0,1,-2].
 *  The inner sum Σ_{d|k} d*c_d is pre‑computed once for all required k. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;   /* a[0..cap-1] */
static int   *f_cache = NULL;   /* Σ_{d|k} d*c_d for 1<=k<cap   */
static size_t a_cap = 0;          /* current allocated size     */

static const int c_table[24] = {
    1, 0, 1, -2, 1, -1, 1, -1, 1, 0, 1, -2,
    1, 0, 1, -1, 1, -1, 1, -2, 1, 0, 1, -2
};

seq_t a(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* we need indices 0..n */
    if (needed > a_cap) {
        /* resize a_cache */
        seq_t *new_a = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
        if (!new_a) return (seq_t)0;
        a_cache = new_a;

        /* resize f_cache */
        int *new_f = (int *)realloc(f_cache, needed * sizeof(int));
        if (!new_f) return (seq_t)0;
        f_cache = new_f;

        /* compute f[1..needed-1] */
        for (size_t k = 1; k < needed; ++k) f_cache[k] = 0;
        for (size_t d = 1; d < needed; ++d) {
            int c = c_table[(d - 1) % 24];
            int val = (int)d * c;
            for (size_t m = d; m < needed; m += d)
                f_cache[m] += val;
        }

        /* set a[0] */
        a_cache[0] = (seq_t)1;

        /* compute a[i] for i = max(1, a_cap) .. needed-1 */
        size_t start = a_cap;
        if (start == 0) start = 1;
        for (size_t i = start; i < needed; ++i) {
            int64_t sum = 0;                /* intermediate signed sum   */
            for (size_t k = 1; k <= i; ++k) {
                sum += (int64_t)f_cache[k] * (int64_t)a_cache[i - k];
            }
            a_cache[i] = (seq_t)(sum / (int64_t)i);
        }
        a_cap = needed;
    }
    return a_cache[(size_t)n];
}
