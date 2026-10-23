/*
 * A175134  a(n) = number of pairs of permutations (p, q) of the same
 *          (arbitrary) size m such that f_m(f_{m-1}(...f_1(1)...)) = n, where
 *          f_i(x) = p_i*x + q_i.
 * https://oeis.org/A175134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 170, u32 n <= 170, u64 n <= 170,
 *            u128 n <= 170, big any n
 * Helpers:   min_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{(p,q) | |p|=|q|, f_m(f_{m-1}(...f_1(1)...)) = n}
 *
 * For a fixed size m, each permutation p and q of {1,…,m} defines
 * f_i(x) = p_i*x + q_i.  The composed value at x = 1 is
 *     V(p,q) = (…((1*p_1 + q_1)*p_2 + q_2)…)*p_m + q_m
 *          = product_{i=1..m} p_i  +  Σ_{k=1..m} q_k * Π_{j>k} p_j.
 *
 * The minimal possible value for a given m is obtained by taking
 * p = (m,m‑1,…,1) (descending) and q = (1,2,…,m) (ascending).
 * That value is A370369(m)+m = A014144(m)+m! .
 *
 * The function enumerates all pairs (p,q) for all m such that
 * minimal_value(m) ≤ n and counts those yielding V(p,q)=n.
 * The m=0 case (empty permutations) contributes the single pair
 * yielding n = 1.
 */

#include <stdbool.h>
#include "../utils/min_value.h"

/* next_permutation for an int array of length n.
   Returns true if a next permutation exists, false otherwise
   (array is left as the last permutation in that case). */
static bool next_perm(int *a, int n)
{
    if (n <= 1) return false;
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] < a[i]) --j;
    int t = a[i]; a[i] = a[j]; a[j] = t;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        t = a[l]; a[l] = a[r]; a[r] = t;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; no term for n=0 */
    seq_t target = (seq_t)n;
    seq_t total = 0;

    /* m = 0 : empty permutations, value = 1 */
    if (target == 1) total++;

    /* upper bound for m: break when minimal value exceeds target */
    for (int m = 1; ; ++m) {
        seq_t min_val = min_value(m);
        if (min_val > target) break;

        int perm_p[7], perm_q[7];
        for (int i = 0; i < m; ++i) perm_p[i] = i + 1;
        do {
            for (int i = 0; i < m; ++i) perm_q[i] = i + 1;
            do {
                seq_t val = 1;
                for (int i = 0; i < m; ++i) {
                    val = (seq_t)perm_p[i] * val + (seq_t)perm_q[i];
                }
                if (val == target) ++total;
            } while (next_perm(perm_q, m));
        } while (next_perm(perm_p, m));
    }

    return total;
}
