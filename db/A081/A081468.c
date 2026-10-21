/*
 * A081468  a(n) is the smallest multiple of n of the type k*n + k*(k+1)/2,
 *          i.e., the smallest sum (n+1) to (n+k) which is divisible by n.
 * https://oeis.org/A081468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest multiple of n of the form k*n + k*(k+1)/2, *
 *            where k>=1 is the smallest integer satisfying k*(k+1)/2 ≡ 0 (mod
 *            n)
 * Range:     u8 n <= 15, u16 n <= 210, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n >= 1. Handle offset 1 automatically. */
    /* Find smallest k such that k*(k+1)/2 is divisible by n. */
    seq_t mod = 0;            /* T_k modulo n */
    idx_t k;                  /* candidate k */

    for (k = 1;; ++k) {
        mod = (mod + (seq_t)k) % (seq_t)n;   /* T_k = T_{k-1} + k */
        if (mod == 0) {
            break;           /* found minimal k */
        }
    }

    /* Compute a(n) = n*k + k*(k+1)/2 */
    seq_t kk = (seq_t)k;
    seq_t prod = kk * (kk + 1) / 2;          /* k*(k+1)/2, exact division */
    seq_t result = (seq_t)n * k + prod;
    return result;
}
