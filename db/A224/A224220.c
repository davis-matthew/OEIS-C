/*
 * A224220  a(n) = smallest number k with property that if the base-n
 *          expansion of k is reversed, the result is a nontrivial multiple of
 *          k.
 * https://oeis.org/A224220
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A224220: for n ≥ 3, a(n) is the smallest positive integer k such that
 * the reversal of the base‑n representation of k is a nontrivial multiple of k.
 * For n = 3 or n + 1 prime, the proven formula is
 *       a(n) = (n² − 1)(n + 1).
 * Otherwise, the minimal solution has exactly two base‑n digits.
 * Let k = a·n + b (0 ≤ a,b < n).  From a·(rev k – k) = 0 we obtain
 *       b·(n − m) = a·(m·n − 1),  where m = b.
 * The smallest solution is
 *       a = (n − m)/g,   b = (m·n − 1)/g
 * with g = gcd(n − m, m·n − 1).  We search for the first such pair.
 * The implementation uses the generic gcd routine from utils/gcd.h
 * and the prime test from utils/is_prime.h.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* undefined for n < 3 – callers must not request such indices */
    if (n < 3u) return 0;

    /* Special case: n = 3 or n + 1 is prime */
    if (n == 3u || is_prime(n + 1u)) {
        seq_t sn = (seq_t)n;
        return (sn * sn - (seq_t)1) * (sn + (seq_t)1);
    }

    /* General case: composite n + 1 – search for two‑digit solution */
    seq_t best = 0;
    seq_t sn = (seq_t)n;
    for (idx_t m = 2u; m < n; ++m) {
        seq_t sm = (seq_t)m;
        seq_t a_num = sn - sm;                /* n – m   */
        seq_t b_den = sm * sn - (seq_t)1;     /* m·n – 1 */
        seq_t g = gcd(a_num, b_den);
        seq_t a = a_num / g;
        seq_t b = b_den / g;
        if (a < sn && b < sn) {
            seq_t k = a * sn + b;
            if (best == 0 || k < best) best = k;
        }
    }
    return best;
}
