/*
 * A117658  Number of solutions to x^(k+1) = x^k mod n for some k >= 1.
 * https://oeis.org/A117658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A117658: Number of solutions to x^(k+1) = x^k (mod n) for some k ≥ 1.
 * Formula: a(n) = ∏_{p^e‖n} (1 + p^(e-1))
 * with a(1) = 1.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition; handle trivial case */
    if (n == 1) return (seq_t)1;

    /* Factor n by trial division (sufficient for n up to 10 000) */
    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Count factor 2 separately */
    if ((temp & 1ULL) == 0) {
        uint64_t cnt = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++cnt;
        }
        /* term = 1 + 2^(cnt-1) */
        seq_t pow = (seq_t)1;
        for (uint64_t i = 1; i < cnt; ++i) pow *= (seq_t)2;
        result *= ((seq_t)1 + pow);
    }

    /* Count odd prime factors */
    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            uint64_t cnt = 0;
            while (temp % i == 0) {
                temp /= i;
                ++cnt;
            }
            /* term = 1 + i^(cnt-1) */
            seq_t pow = (seq_t)1;
            for (uint64_t j = 1; j < cnt; ++j) pow *= (seq_t)i;
            result *= ((seq_t)1 + pow);
        }
    }

    /* If remaining temp is a prime > 1 */
    if (temp > 1) {
        /* exponent 1 → term = 1 + temp^(0) = 2 */
        result *= ((seq_t)2);
    }

    return result;
}
