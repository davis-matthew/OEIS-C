/*
 * A069095  Jordan function J_10(n).
 * https://oeis.org/A069095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = n^10 * Π_{p|n} (1 - 1/p^10) (Jordan function J_10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple power function for seq_t (works for arbitrary–precision types as well) */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return 1;

    /* Factor n using simple trial division */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {               /* Remaining prime factor */
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute the product ∏ (p^(10e) – p^(10(e–1))) */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p10e        = pow_seq_t((seq_t)primes[i], 10ULL * exps[i]);
        seq_t p10e_minus1;
        if (exps[i] == 0) {
            p10e_minus1 = 1;   /* Should not occur as e ≥ 1 */
        } else {
            p10e_minus1 = pow_seq_t((seq_t)primes[i], 10ULL * (exps[i] - 1U));
        }
        result *= (p10e - p10e_minus1);
    }

    return result;
}
