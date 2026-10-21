/*
 * A069171  Numbers k such that gcd(k, 2^k-1) = 3.
 * https://oeis.org/A069171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5201, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by checking multiples of 3 for gcd(k, 2^k-1) == 3. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n ≤ 0 */

    const seq_t three = (seq_t)3;
    idx_t      found  = 0;

    for (idx_t k = 3; found < n; k += 3) {
        seq_t k_seq = (seq_t)k;

        /* r = 2^k (mod k) */
        seq_t r = mod_pow((seq_t)2, k, k_seq);

        /* compute (2^k - 1) (mod k) */
        seq_t r_minus1;
        if (r == (seq_t)0) {
            r_minus1 = k_seq - (seq_t)1;   /* k-1 */
        } else {
            r_minus1 = r - (seq_t)1;
        }

        seq_t g = gcd(k_seq, r_minus1);
        if (g == three) {
            ++found;
            if (found == n) {
                return k_seq;
            }
        }
    }
    return 0;    /* unreachable for valid n */
}
