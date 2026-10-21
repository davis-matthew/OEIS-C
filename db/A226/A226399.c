/*
 * A226399  Number of nondecreasing -n..n vectors of length 2 whose dot
 *          product with some nonincreasing -n..n vector equals 2.
 * https://oeis.org/A226399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nondecreasing vectors [a0 , a1] with –n ≤ a0 ≤
 *            a1 ≤ n * for which there exists a nonincreasing vector [b0 , b1]
 *            with –n ≤ b0 , b1 ≤ n * such that a0·b0 + a1·b1 = 2
 * Range:     u8 n <= 12, u16 n <= 207, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   div_ceil, div_floor, egcd_pos, k_interval, llabsll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/div_ceil.h"
#include "../utils/div_floor.h"
#include "../utils/egcd_pos.h"
#include "../utils/k_interval.h"
#include "../utils/llabsll.h"

/* Euclidean gcd for non‑negative numbers */
static inline long long my_gcd(long long a, long long b)
{
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    /* n may be up to several thousands; use int for loop indices */
    int N = (int)n;
    seq_t count = 0;

    for (int a0 = -N; a0 <= N; ++a0) {
        for (int a1 = a0; a1 <= N; ++a1) {          /* a0 ≤ a1 : nondecreasing */
            if (a0 == 0 && a1 == 0) continue;        /* dot product cannot be 2 */

            long long abs_a0 = llabsll(a0);
            long long abs_a1 = llabsll(a1);
            long long g = my_gcd(abs_a0, abs_a1);
            if (g == 0 || (2LL % g) != 0) continue;  /* gcd must divide 2 */

            long long p = a0 / g;
            long long q = a1 / g;
            long long R = 2LL / g;                   /* integer */

            /* find one particular solution (b0_0 , b1_0) for p*b0 + q*b1 = R */
            long long x, y;
            egcd_pos(llabsll(p), llabsll(q), &x, &y);
            if (p < 0) x = -x;
            if (q < 0) y = -y;
            long long b0_0 = x * R;
            long long b1_0 = y * R;

            long long step_q = q;          /* coefficient for k in b0 */
            long long step_p = -p;         /* coefficient for k in b1 */

            /* k bounds from b0 */
            long long k_min, k_max;
            if (!k_interval(b0_0, step_q, -N, N, &k_min, &k_max)) continue;
            long long k_low = k_min;
            long long k_high = k_max;

            /* k bounds from b1 */
            if (!k_interval(b1_0, step_p, -N, N, &k_min, &k_max)) continue;
            if (k_min > k_low) k_low = k_min;
            if (k_max < k_high) k_high = k_max;

            /* inequality b0 >= b1 */
            long long diff = step_q - step_p;      /* = p + q */
            long long need = b1_0 - b0_0;          /* RHS of inequality */

            if (diff > 0) {
                long long lk = div_ceil(need, diff);
                if (lk > k_low) k_low = lk;
            } else if (diff < 0) {
                long long hk = div_floor(need, diff);
                if (hk < k_high) k_high = hk;
            } else { /* diff == 0 */
                if (need > 0) continue;           /* inequality impossible */
            }

            if (k_low <= k_high) ++count;
        }
    }
    return count;
}
