/*
 * A004123  Number of generalized weak orders on n points.
 * https://oeis.org/A004123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = b(n-1), where b(m) = Sum_{k=0..m} U(m,k), U(m,k) =
 *            2^k*k!*Stirling2(m,k), U(m,k) = k*U(m-1,k) + 2*k*U(m-1,k-1),
 *            U(0,0) = 1 (e.g.f. of b: 1/(3-2*exp(x)))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *U = NULL;   /* current triangle row U(m, .) */
    static seq_t *B = NULL;   /* B[m] = b(m) */
    static idx_t cap = 0;
    static idx_t m = 0;

    idx_t target = n - 1;

    if (U == NULL) {
        cap = target + 2;
        if (cap < 16) cap = 16;
        U = alloc_zero(cap);
        B = alloc_zero(cap);
        U[0] = 1;
        B[0] = 1;
        m = 0;
    }
    if (target + 1 >= cap) {
        idx_t newcap = 2 * cap;
        if (newcap < target + 2) newcap = target + 2;
        U = grow_zero(U, cap, newcap);
        B = grow_zero(B, cap, newcap);
        cap = newcap;
    }

    while (m < target) {
        m++;
        seq_t sum = 0;
        for (idx_t k = m; k >= 1; k--) {
            U[k] = U[k] * k + U[k - 1] * (2 * k);
            sum += U[k];
        }
        U[0] = 0;
        B[m] = sum;
    }

    return B[target];
}
