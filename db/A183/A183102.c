/*
 * A183102  a(n) = product of powerful divisors d of n.
 * https://oeis.org/A183102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of powerful divisors d of n. A divisor d of n is
 *            powerful if, for every prime p dividing d, the exponent of p in
 *            d is either 0 or at least 2
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 215, u64 n <= 863,
 *            u128 n <= 3455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* --- local helper: factorize n into primes and exponents ----------------- */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[i] = p;
            exps[i]   = e;
            ++i;
        }
    }
    if (n > 1) {
        primes[i] = n;
        exps[i]   = 1;
        ++i;
    }
    *cnt = i;
}

/* --- fast exponentiation: base^exp --------------------------------- */
static seq_t pow_seq(seq_t base, seq_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n >= 1 by definition; treat n==0 as 1 for safety */
    if (n == 0) return (seq_t)1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    /* for n==1 (cnt==0) the product is 1 */
    if (cnt == 0) return (seq_t)1;

    /* compute total number of combinations C_total = Π e_i */
    seq_t C_total = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        C_total *= (seq_t)exps[i];
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* sum of allowed exponents for this prime: 2+3+...+e
           equals e*(e+1)/2 - 1   (works also for e==1, gives 0) */
        seq_t sum_f = (seq_t)e * (seq_t)(e + 1) / 2 - 1;
        if (sum_f == 0) continue;  /* no contribution when e==1 */

        seq_t C_other = C_total / (seq_t)e;          /* combinations of other primes */
        seq_t exponent = C_other * sum_f;            /* total exponent of this prime */
        seq_t pow = pow_seq((seq_t)primes[i], exponent);
        result *= pow;
    }
    return result;
}
