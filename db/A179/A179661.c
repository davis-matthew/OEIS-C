/*
 * A179661  Triangle read by rows: T(n,k) is the largest least common multiple
 *          of any k-element subset of the first n positive integers.
 * https://oeis.org/A179661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179661: Triangle read by rows – T(n,k) = max{ lcm(x1,…,xk) ; 0 < xi ≤ n }.
   This implementation computes the term a(n) (offset 1) by first determining
   the row r and column k that correspond to the linear index n, then
   selecting k numbers from {1,…,r} that maximize the lcm greedily.
   The routine works for all r ≤ 141 (the maximal row for n≤10000) and
   thus satisfies the required time bound. */
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r*(r+1))/2 < n) r++;

    /* Column k within that row */
    idx_t before = (r*(r-1))/2;      /* index of first element in row r */
    idx_t k = n - before;            /* 1‑based position in the row */

    /* Greedy selection of k numbers to maximize the lcm */
    bool chosen[200] = {0};          /* r ≤ 141, so array size 200 is safe */
    seq_t cur = (seq_t)1;            /* lcm of empty set */

    for (idx_t t = 0; t < k; ++t) {
        seq_t best = 0;
        idx_t best_i = 0;

        for (idx_t i = r; i >= 1; --i) {
            if (chosen[i]) continue;
            seq_t num = (seq_t)i;
            seq_t g   = gcd(cur, num);
            seq_t cand = (cur / g) * num;
            if (cand > best) {
                best   = cand;
                best_i = (idx_t)i;
            }
        }
        cur = best;
        chosen[best_i] = true;
    }
    return cur;
}
