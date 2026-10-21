/*
 * A221876  T(n,k) is the number of order-preserving full contraction mappings
 *          (of an n-chain) with exactly k fixed points.
 * https://oeis.org/A221876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if n is a triangular number n = t(t+1)/2; otherwise
 *            a(n) = A045623(s - (n - T(s-1)) + 1) where s = smallest integer
 *            with T(s) >= n and T(k) = k(k+1)/2. A045623(1)=1, A045623(2)=2,
 *            A045623(m) = 2*A045623(m-1) + 2^(m-3) for m >= 3
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n){
    /* Precompute A045623 up to a safe limit */
    static seq_t a045[210];
    static bool init = false;
    if (!init){
        init = true;
        a045[1] = 1;
        a045[2] = 2;
        for (idx_t i = 3; i < 210; ++i){
            a045[i] = 2*a045[i-1] + pow2(i-3);
        }
    }

    if (n <= 1) return 1;   /* a(1) = 1 */

    /* Find s such that T(s) = s*(s+1)/2 >= n */
    idx_t s = 1;
    while (s*(s+1)/2 < n) ++s;

    /* If n is triangular, value is 1 */
    if (s*(s+1)/2 == n) return 1;

    /* Position within the block between two triangular numbers */
    idx_t prev = s*(s-1)/2;         /* T(s-1) */
    idx_t offset = n - prev;        /* 1 .. s-1 */
    idx_t m = s - offset + 1;       /* corresponding A045623 index */

    return a045[m];
}
