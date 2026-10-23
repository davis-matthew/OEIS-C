/*
 * A108414  Number of integer k:s for which max{x^(k-x) | x integer, 0<x<k} =
 *          n^(k-n).
 * https://oeis.org/A108414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A108414 – Count of k > n for which max{x^(k-x)} = n^(k-n)
 *
 * Derivation:
 *   For a fixed n the maximum of f(x)=x^(k-x) (1≤x≤k-1) is attained
 *   at the integer x which satisfies
 *        (n/(n‑1))^(k‑n) ≥ n‑1            (1)
 *        ( (n+1)/n )^(k‑n) ≤ n+1          (2)
 *   Let t = k‑n.  Inequalities (1) and (2) give bounds on t:
 *        t ≥ ceil( ln(n‑1) / ln(n/(n‑1)) )
 *        t ≤ floor( ln(n+1) / ln((n+1)/n) )
 *   Since k must be strictly greater than n, we also require t ≥ 1.
 *   The number of integers t satisfying both bounds is
 *        max(0, t_high – t_low + 1).
 *   For n = 1 the only k that can work is k = 1, giving a(1)=1.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    long double nd = (long double)n;

    /* logarithmic bounds */
    long double ln_n_minus_1 = logl(nd - 1.0L);
    long double ln_n_plus_1  = logl(nd + 1.0L);

    long double delta = logl(nd / (nd - 1.0L));   /* ln(n/(n-1))  */
    long double epsilon = logl((nd + 1.0L) / nd); /* ln((n+1)/n) */

    long double tl_raw = ln_n_minus_1 / delta;
    long double th_raw = ln_n_plus_1 / epsilon;

    /* ceil/floor with guard against floating‑point error */
    idx_t t_low = (idx_t)ceill(tl_raw - 1e-16L);
    idx_t t_high = (idx_t)floorl(th_raw + 1e-16L);

    if (t_low == 0) t_low = 1;                    /* k must exceed n */

    seq_t result = 0;
    if (t_high >= t_low) result = (seq_t)(t_high - t_low + 1);

    return result;
}
