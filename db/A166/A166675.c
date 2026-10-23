/*
 * A166675  Totally multiplicative sequence with a(p) = 7p+2 for prime p.
 * https://oeis.org/A166675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} (7p+2)^e
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;                     /* defined only for n ≥ 1 */

    seq_t result = (seq_t)1;
    uint64_t num = (uint64_t)n;

    /* trial‑division factorisation */
    for (uint64_t p = 2; p <= num / p; ++p) {
        if (num % p == 0) {
            uint64_t exp = 0;
            while (num % p == 0) {
                num /= p;
                ++exp;
            }
            /* compute (7p+2)^exp */
            seq_t term = (seq_t)7 * (seq_t)p + (seq_t)2;
            seq_t pow_val = (seq_t)1;
            seq_t base = term;
            uint64_t e = exp;
            while (e) {
                if (e & 1) pow_val = pow_val * base;
                base = base * base;
                e >>= 1;
            }
            result = result * pow_val;
        }
    }

    /* any remaining prime factor (exponent 1) */
    if (num > 1) {
        seq_t term = (seq_t)7 * (seq_t)num + (seq_t)2;
        result = result * term;
    }

    return result;
}
