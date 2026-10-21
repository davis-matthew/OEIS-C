/*
 * A068218  Triangle of numbers of square lattice walks that start and end at
 *          origin after 2k steps and contain exactly r steps to the east, not
 *          touching origin at intermediate stages.
 * https://oeis.org/A068218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   find_k_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of square lattice walks  
   a(n) = T(k,r) where k(k+1)/2 ≤ n < (k+1)(k+2)/2 and r = n - k(k+1)/2
   T(k,r) is defined by:
     T(0,0) = 1
     T(1,0) = 2, T(1,1) = 2
     if k > 2r:
          T(k,r) = ( T(k-1,r) - T(k-1,r-1) ) * 2*(2k-3)/(k-2r)
     else:
          T(k,r) = A069466(k,r)
                 - Σ_{i=1}^{k-1} Σ_{j=max(0,r-(k-i))}^{min(i,r)}
                   T(i,j) * A069466(k-i,r-j)
   A069466(k,r) = C(2k,r) * C(2k-r,r) * C(2k-2r,k-r)
   (C denotes the binomial coefficient)
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/find_k_r.h"

#define MAX_K 200   /* sufficient for n up to ≈ 20000 */

static seq_t T[MAX_K + 1][MAX_K + 1];
static seq_t A[MAX_K + 1][MAX_K + 1];
static bool triangle_ready = false;

/* multiplicative binomial coefficient C(n,r) */
static inline seq_t binom(idx_t n, idx_t r)
{
    if (r > n - r) r = n - r;
    seq_t res = 1;
    for (idx_t i = 1; i <= r; ++i)
        res = (res * (n - r + i)) / i;
    return res;
}

/* precompute binomial table A[k][r] = A069466(k,r) */
static void init_A(void)
{
    for (idx_t k = 0; k <= MAX_K; ++k) {
        for (idx_t r = 0; r <= k; ++r) {
            /* A069466(k,r) = C(2k,r) * C(2k-r,r) * C(2k-2r,k-r) */
            seq_t v = binom(2 * k, r);
            v = (v * binom(2 * k - r, r)) / 1;
            v = (v * binom(2 * k - 2 * r, k - r)) / 1;
            A[k][r] = v;
        }
    }
}

/* compute the triangle T(k,r) */
static void init_triangle(void)
{
    init_A();
    T[0][0] = 1;
    T[1][0] = 2;
    T[1][1] = 2;

    for (idx_t k = 2; k <= MAX_K; ++k) {
        for (idx_t r = 0; r <= k; ++r) {
            if (k > 2 * r) {                     /* recurrence */
                seq_t diff = T[k - 1][r] - (r > 0 ? T[k - 1][r - 1] : 0);
                seq_t num = diff * (seq_t)(2 * (2 * k - 3));
                seq_t den = (seq_t)(k - 2 * r);
                T[k][r] = num / den;
            } else {                             /* sum formula */
                seq_t sum = 0;
                for (idx_t i = 1; i < k; ++i) {
                    idx_t j_min = 0;
                    if (r > k - i) j_min = r - (k - i);
                    idx_t j_max = (r < i) ? r : i;
                    for (idx_t j = j_min; j <= j_max; ++j) {
                        sum += T[i][j] * A[k - i][r - j];
                    }
                }
                T[k][r] = A[k][r] - sum;
            }
        }
    }
    triangle_ready = true;
}

seq_t a(idx_t n)
{
    if (!triangle_ready)
        init_triangle();

    idx_t k, r;
    find_k_r(n, &k, &r);
    return T[k][r];
}
