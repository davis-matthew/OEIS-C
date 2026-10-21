/*
 * A072028  Swap twin prime pairs of form (4*k+1,4*k+3) in prime factorization
 *          of n.
 * https://oeis.org/A072028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p)^e, where a(p) = p + 2 if p ≡ 1 (mod 4) and
 *            p + 2 is prime, p - 2 if p ≡ 3 (mod 4) and p – 2 is prime, p
 *            otherwise. (a(1) = 1)
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Simple primality test for 64‑bit values.  Works for the small
   numbers needed in this routine (at most a few 10⁴).  */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;          /* 2 or 3 */
    if ((x & 1ULL) == 0ULL) return false;
    if (x % 3ULL == 0ULL) return x == 3;
    for (uint64_t d = 5; d*d <= x; d += 6) {
        if (x % d == 0ULL) return false;
        if (x % (d + 2ULL) == 0ULL) return false;
    }
    return true;
}

/* Factorise n into prime powers.  Stores primes in pr[], exponents in
   ex[], and the number of distinct primes in *cnt.  Primes are in
   increasing order.  */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }
    for (uint64_t d = 2; d*d <= n; d += (d == 2 ? 1 : 2)) {
        if (n % d == 0ULL) {
            uint64_t e = 0;
            while (n % d == 0ULL) {
                n /= d;
                ++e;
            }
            pr[c] = d;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {        /* remaining prime factor */
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factor n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    /* compute the product of swapped primes ^ exponent */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t rem = p;

        if ((p & 3ULL) == 1ULL) {            /* p ≡ 1 (mod 4) */
            if (is_prime_uint64(p + 2ULL))
                rem = p + 2ULL;
        } else if ((p & 3ULL) == 3ULL) {      /* p ≡ 3 (mod 4) */
            if (is_prime_uint64(p - 2ULL))
                rem = p - 2ULL;
        }

        for (uint64_t k = 0; k < e; ++k)
            result *= (seq_t)rem;
    }
    return result;
}
