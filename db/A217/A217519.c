/*
 * A217519  Base-2 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A217519
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ord_2(n / 2^v2(n)) + (2^v2(n) - 1) for n >= 2 where
 *            v2(n) is the exponent of 2 in n (number of trailing zeros), and
 *            ord_2(m) is the least positive k with 2^k ≡ 1 (mod m), m odd.
 *            This formula covers all cases: - If n is a power of two (m = 1,
 *            ord_2(1) = 1) → a(n) = 2^(v2(n)+1) – 1. - If...
 * Range:     u8 n <= 18, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Validated: standard, pass, 69 terms through n = 121 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long order2_odd(unsigned long long mod)
{
    /* mod is odd, mod >= 3 */
    unsigned long long pow = 2 % mod;
    unsigned long long k = 1;
    while (pow != 1ULL) {
        pow = (pow * 2ULL) % mod;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    /* compute v2(n) – number of trailing zero bits */
    unsigned int t = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        ++t;
    }
    idx_t m = n >> t;          /* odd part of n */
    unsigned long long ord;    /* multiplicative order of 2 modulo m */

    if (m == 1ULL) {
        /* n is a power of two */
        ord = 1ULL;
    } else {
        ord = order2_odd(m);
    }

    seq_t pow2t = ((seq_t)1) << t;     /* 2^t */
    seq_t result = ((seq_t)n) * (seq_t)ord + (pow2t - (seq_t)1);
    return result;
}
