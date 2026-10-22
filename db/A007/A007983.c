/*
 * A007983  Number of non-Abelian metacyclic groups of order p^n (p odd).
 * https://oeis.org/A007983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Derived from pattern in differences d(n) = a(n+1) - a(n);
 *            computes a(n) by accumulating d values
 * Range:     u8 n <= 23, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   b_seq, c_seq, compute_floor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_seq.h"
#include "../utils/c_seq.h"
#include "../utils/compute_floor_sum.h"

seq_t a(idx_t n) {
    if (n == 1 || n == 2) return 0;
    
    seq_t sum = 0;
    if (n % 2 == 0) {
        idx_t m = n / 2;
        for (idx_t j = 1; j <= m; j++) {
            sum += b_seq(j);
        }
        for (idx_t j = 1; j < m; j++) {
            sum += c_seq(j);
        }
    } else {
        idx_t m = (n - 1) / 2;
        for (idx_t j = 1; j <= m; j++) {
            sum += b_seq(j) + c_seq(j);
        }
    }
    return sum;
}
