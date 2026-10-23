/*
 * A119584  a(n) = Sum_{k=1..phi(n)-1} t(n,k)*t(n,k+1), where t(n,k) is the
 *          k-th positive integer which is coprime to n and phi(n) is the
 *          number of positive integers which are <= n and are coprime to n.
 * https://oeis.org/A119584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..phi(n)-1} t(n,k)*t(n,k+1) where t(n,k) is the
 *            k-th positive integer coprime to n (totative)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"

/* Euclidean algorithm for idx_t */

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t prev = 0;
    bool havePrev = false;
    for (idx_t i = 1; i <= n; ++i) {
        if (gcd_idx(i, n) == 1) {
            if (havePrev) {
                sum += ((seq_t)prev) * (seq_t)i;
            }
            prev = i;
            havePrev = true;
        }
    }
    return sum;
}
