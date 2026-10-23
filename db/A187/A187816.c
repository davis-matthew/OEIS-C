/*
 * A187816  Triangle read by rows in which row n lists the first 2^(n-1) terms
 *          of A006519 in nonincreasing order, n >= 1.
 * https://oeis.org/A187816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, changed
 * Formula:   a(n) = 2^(r-1-ceil(log2(k))) where r = ceil(log2(n+1)) k = n -
 *            (2^(r-1) - 1) i.e. a(n) is the (n‑th) entry of triangle T with
 *            T(r,k) = 2^(r-1-ceil(log2(k))), 1 ≤ k ≤ 2^(r-1)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Computes entry a(n) of A187816, n ≥ 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for out‑of‑bounds */
    /* Find r = ceil(log2(n+1)) : smallest r with 2^r ≥ n+1 */
    idx_t r = 0;
    idx_t pow = 1;
    idx_t n1 = n + 1;                     /* n+1 ≤ 2^64 for the tested range */
    while (pow < n1) {                    /* loop at most 64 iterations */
        pow <<= 1;
        r++;
    }
    /* previous row total = 2^(r-1) - 1 */
    idx_t prev_total = (pow >> 1) - 1;
    idx_t k = n - prev_total;             /* 1‑based position in row r */

    /* ceil(log2(k)) : smallest t with 2^t ≥ k */
    idx_t t = 0;
    idx_t p = 1;
    while (p < k) {
        p <<= 1;
        t++;
    }
    idx_t exp = r - 1 - t;                /* exponent of 2 in the result */
    return pow2(exp);                     /* pow2 is defined in utils/ */
}
