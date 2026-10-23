/*
 * A193376  T(n,k) = number of ways to place any number of 2 X 1 tiles of k
 *          distinguishable colors into an n X 1 grid; array read by
 *          descending antidiagonals, with n, k >= 1.
 * https://oeis.org/A193376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 129, u32 n <= 342, u64 n <= 976,
 *            u128 n <= 2829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A193376
 *
 *  T(n,k) = number of ways to place any number of 2×1 tiles of
 *  k distinguishable colors into an n×1 grid.
 *
 *  a(n) is the (n‑th) entry of the table T(n,k) read by descending
 *  antidiagonals:
 *   - first element = T(1,1)
 *   - for each diagonal d = 1,2,3,...
 *        list the pairs (n,k) with n + k – 1 = d
 *        in increasing order of n
 *   - d = n + k – 1
 *
 *  The recurrence for fixed k is
 *      T(0,k) = 1,   T(1,k) = 1
 *      T(m,k) = T(m‑1,k) + k * T(m‑2,k)   (m ≥ 2)
 *
 *  Because all values for a given (n,k) can be obtained from this
 *  recurrence, a straightforward iterative computation is sufficient.
 *
 *  The function below implements the following steps:
 *
 *  1. Determine the diagonal d such that d(d+1)/2 ≥ n.
 *     (d is the smallest integer with this property.)
 *
 *  2. Compute the offset inside that diagonal:
 *          offset = n – (d‑1)d/2   (1 ≤ offset ≤ d)
 *
 *  3. Recover the table coordinates:
 *          N = offset
 *          K = d – offset + 1
 *
 *  4. Evaluate T(N,K) by the recurrence above.
 */

#include <stdint.h>
#include <stdbool.h>

extern unsigned long long _floor_isqrt_u64(unsigned long long);

seq_t a(idx_t n)
{
    /* ---------- Step 1: find diagonal d ----------
     * Solve d(d+1)/2 ≥ n.  Use binary search on idx_t.  */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* compute mid*(mid+1)/2 safely in 128 bits to avoid overflow */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* ---------- Step 2: compute offset inside diagonal ----------
     * triangular number of previous diagonal:
     *    prev = (d-1)d/2 */
    unsigned __int128 prev_tri = (unsigned __int128)(d - 1) * d / 2;
    idx_t offset = (idx_t)(n - (idx_t)prev_tri);   /* 1-based offset */

    /* ---------- Step 3: recover coordinates ---------- */
    idx_t N = offset;                /* n in T(n,k) */
    idx_t K = d - offset + 1;        /* k in T(n,k) */

    /* ---------- Step 4: evaluate T(N,K) ---------- */
    if (N == 1)                /* base case T(1,K) = 1 */
        return (seq_t)1;

    /* T(0,K) = 1, T(1,K) = 1 */
    seq_t prev_prev = (seq_t)1;   /* T(0,K) */
    seq_t prev      = (seq_t)1;   /* T(1,K) */

    /* iterative recurrence for N >= 2 */
    for (idx_t i = 2; i <= N; ++i) {
        seq_t cur = prev + ((seq_t)K) * prev_prev;
        prev_prev = prev;
        prev = cur;
    }
    return prev;
}
