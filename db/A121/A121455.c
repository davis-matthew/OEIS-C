/*
 * A121455  Expansion of q*(phi(-q)psi(q^4))^2 in powers of q where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A121455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 0 if n % 4 == 0 * a(n) = sigma(n) if n % 2 == 1 * a(n) =
 *            -4 * sigma(n/2) if n % 4 == 2 * * where sigma(m) is the sum of
 *            all positive divisors of m
 * Range:     i8 n <= 12, i16 n <= 672, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   factor_u64, my_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/my_pow_seq.h"

/* sum of divisors sigma(m) for a uint64_t m */
static seq_t sigma_u64(uint64_t m)
{
    /* factor_u64 uses three arrays; 64 entries is enough for any n < 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)(exps[i] + 1);          /* need e+1 */
        seq_t term = my_pow_seq(p, e);
        term = (term - 1) / (p - 1);             /* (p^(e+1)-1)/(p-1) */
        res *= term;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n % 4 == 0)            /* n is multiple of 4 */
        return 0;
    if (n % 2 == 0) {          /* n ≡ 2 (mod 4) */
        uint64_t half = n / 2;
        seq_t s = sigma_u64(half);
        return -((seq_t)4 * s);
    }                          /* n is odd */
    return sigma_u64(n);
}
