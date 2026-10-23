/*
 * A040087  Primes p such that x^3 = 21 has a solution mod p.
 * https://oeis.org/A040087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 21 is a cubic residue modulo p. The code
 *            tests primes sequentially, using the fact that for p≡2 (mod 3)
 *            every element has a unique cube root, for p≡1 (mod 3) that 21 is
 *            a cube iff 21^((p–1)/3) ≡ 1 (mod p), and for p = 3, 7 the
 *            congruence is trivial
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Simple modular exponentiation for seq_t. */
static seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1u)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1u;
    }
    return result;
}

/* Dynamic array of primes found so far (type idx_t). */
static idx_t *prime_tbl = NULL;
static idx_t prime_cnt = 0;
static idx_t prime_cap = 0;

/* Ensure the array has at least new_cap elements. */
static void ensure_prime_cap(idx_t new_cap)
{
    if (new_cap <= prime_cap)
        return;
    if (prime_tbl == NULL)
        prime_tbl = (idx_t *)malloc(new_cap * sizeof(idx_t));
    else
        prime_tbl = (idx_t *)realloc(prime_tbl, new_cap * sizeof(idx_t));
    prime_cap = new_cap;
}

/* Test whether n is prime using trial division by known primes. */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2u)
        return false;
    if (prime_tbl == NULL)
        ensure_prime_cap(16);
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_tbl[i];
        if (p > n / p)
            break;
        if (n % p == 0)
            return false;
    }
    /* n is prime; store it for future tests. */
    if (prime_cnt == prime_cap)
        ensure_prime_cap(prime_cap * 2 + 8);
    prime_tbl[prime_cnt++] = n;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;           /* undefined, but guard */
    idx_t found = 0u;
    for (idx_t p = 2u; found < n; ++p)
    {
        if (!is_prime_candidate(p))
            continue;
        bool ok;
        if (p == 3u || p == 7u)
            ok = true;
        else if (p % 3u == 2u)
            ok = true;
        else
        {
            seq_t res = mod_pow_seq((seq_t)21, (p - 1u) / 3u, (seq_t)p);
            ok = (res == 1);
        }
        if (ok)
        {
            ++found;
            if (found == n)
                return (seq_t)p;
        }
    }
    return 0; /* unreachable */
}
