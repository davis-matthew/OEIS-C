/*
 * A081239  #{(i,j): mu(i)*mu(j) = 0, 1<=i,j<=n}, where mu=A008683 (Moebius
 *          function).
 * https://oeis.org/A081239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n² – (A013928(n))², where A013928(n) counts the
 *            square‑free integers ≤ n. Thus a(n) is the number of pairs (i ,
 *            j) , 1 ≤ i, j ≤ n, for which μ(i)·μ(j) = 0
 * Range:     u8 n <= 21, u16 n <= 324, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   mobius_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_single.h"

/*---- integer square root (floor) --------------------------------*/ 
static idx_t floor_sqrt(idx_t x) {
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) { ans = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return ans;
}

/*----------------------- main sequence routine --------------------*/ 
seq_t a(idx_t n)
{
    /* a(n) = n² – (square‑free count ≤ n)² */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    idx_t limit = floor_sqrt(n);          /* maximum k with k² ≤ n   */
    seq_t sqf    = 0;                     /* square‑free count       */

    for (idx_t k = 1; k <= limit; ++k) {
        int mu_k = mobius_single(k);
        if (mu_k == 0) continue;         /* skip non‑square‑free k  */
        idx_t denom = k * k;              /* k², safe: k² ≤ n ≤ 2⁶⁴‑1*/
        seq_t term = (seq_t)(n / denom);  /* floor(n / k²)           */
        if (mu_k == 1) sqf += term;
        else           sqf -= term;       /* mu_k == –1 */
    }

    seq_t sqf_sq = sqf * sqf;
    return n_sq - sqf_sq;
}
