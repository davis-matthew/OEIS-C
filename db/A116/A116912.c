/*
 * A116912  In prime factorization of n replace all primes of form k*6+1 with
 *          k*6+5 and primes of form k*6+5 with k*6+1.
 * https://oeis.org/A116912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (mapped(p))^e, where p = 2 or 3 → mapped(p)=p;
 *            p ≡ 1 (mod 6) → mapped(p)=p+4; p ≡ 5 (mod 6) → mapped(p)=p–4.
 *            The mapped value is factored into primes and its prime powers
 *            are multiplied into the result
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_and_apply, mul_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_and_apply.h"
#include "../utils/mul_prime_power.h"

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t r = 1;
    for (uint64_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    seq_t res = 1;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            uint64_t cnt = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++cnt;
            }
            if (d == 2 || d == 3) {
                mul_prime_power(&res, d, cnt);
            } else {
                uint64_t mapped = (d % 6 == 1) ? d + 4 : d - 4;
                factor_and_apply(mapped, cnt, &res);
            }
        }
    }

    if (tmp > 1) {
        if (tmp == 2 || tmp == 3) {
            mul_prime_power(&res, tmp, 1);
        } else {
            uint64_t mapped = (tmp % 6 == 1) ? tmp + 4 : tmp - 4;
            factor_and_apply(mapped, 1, &res);
        }
    }

    return res;
}
