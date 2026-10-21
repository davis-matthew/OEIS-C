/*
 * A077083  Length of iteration until a fixed point is reached when
 *          phi(sigma[n]+phi(n)) = A077080(n) is iterated with initial value
 *          of n.
 * https://oeis.org/A077083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Length[FixedPointList[ A077080, n]]-1, where A077080(n) =
 *            φ(σ(n)+φ(n)), φ is Euler's totient and σ is the sum of divisors
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Prime factorisation of a 64‑bit integer.  */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Euler's totient function. */
static idx_t phi_of(idx_t n)
{
    if (n == 0) return 0;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    idx_t res = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        res -= res / p;           /* res * (p-1) / p  */
    }
    return res;
}

/* Sum of divisors function σ(n).  */
static idx_t sigma_of(idx_t n)
{
    if (n == 0) return 0;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    idx_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p    = primes[i];
        uint64_t e    = exps[i];
        idx_t   p_pow = 1;
        for (uint64_t k = 0; k <= e; ++k) p_pow *= p;   /* p^(e+1) */
        idx_t term = (p_pow - 1) / (p - 1);
        result *= term;
    }
    return result;
}

/* Next value of the iteration:  φ(σ(x)+φ(x))   */
static idx_t next_value(idx_t x)
{
    idx_t phi_x = phi_of(x);
    idx_t sigma_x = sigma_of(x);
    idx_t sum = sigma_x + phi_x;
    return phi_of(sum);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t  curr = n;
    seq_t  len  = 1;          /* include the starting value */
    while (1) {
        idx_t next = next_value(curr);
        if (next == curr) break;
        curr = next;
        ++len;
    }
    return len;
}
