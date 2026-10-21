/*
 * A062378  n divided by largest cubefree factor of n.
 * https://oeis.org/A062378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^(max(e−2,0)) (OEIS A062378)
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, not used in tests */

    uint64_t num = (uint64_t)n;    /* n fits in 64 bits */
    seq_t result = 1;

    /* trial division factorisation */
    uint64_t p = 2;
    while (p <= num / p) {        /* p*p <= num without overflow */
        if (num % p == 0) {
            uint64_t exp = 0;
            while (num % p == 0) {
                num /= p;
                exp++;
            }
            if (exp > 2) {
                uint64_t e = exp - 2;
                seq_t mul = pow_seq_t((seq_t)p, e);
                result *= mul;
            }
        }
        if (p == 2) p = 3; else p += 2;  /* skip even numbers after 2 */
    }

    /* if a prime factor remains, its exponent is 1 (≤2) → no contribution */
    return result;
}
