/*
 * A173484  a(n) = the smallest number ending in n+1 zeros divisible by n.
 * https://oeis.org/A173484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^(n+1) * (n / gcd(n, 10^(n+1)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10, pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest number ending in n+1 zeros that is divisible by n */
#include "../utils/pow10.h"
#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* For safety (n should be >= 1 in OEIS definition) */
    if (n == 0) return 0;

    /* Power of ten with exponent n+1 */
    seq_t ten_pow = pow10(n + 1);

    /* Compute v2 and v5 the exponent of 2 and 5 in n */
    idx_t v2 = 0, v5 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {       /* extract factor 2 */
        ++v2;
        tmp >>= 1;
    }

    tmp = n;
    while (tmp % 5ULL == 0) {          /* extract factor 5 */
        ++v5;
        tmp /= 5;
    }

    /* g2 = min(v2, n+1), g5 = min(v5, n+1) */
    idx_t g2 = v2 < (n + 1) ? v2 : (n + 1);
    idx_t g5 = v5 < (n + 1) ? v5 : (n + 1);

    /* t = n / (2^g2 * 5^g5)  (missing prime factors of n) */
    seq_t t = (seq_t)n;
    if (g2) t /= pow2(g2);            /* divide by 2^g2 */
    if (g5) t /= pow_int(5, g5);      /* divide by 5^g5 */

    /* Result: 10^(n+1) * t */
    return ten_pow * t;
}
