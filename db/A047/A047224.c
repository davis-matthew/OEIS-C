/*
 * A047224  Smallest k>n where n is a square modulo k.
 * https://oeis.org/A047224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest k > n such that n is a quadratic residue modulo k
   (i.e. x^2 ≡ n (mod k) has a solution).  The function is
   computed by exhaustive search and a per‑prime‑power quadratic‑residue
   test. */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* modular exponentiation: (base^exp) mod mod, all values fit
   into uint64_t because mod ≤ 2^32. */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* test whether n (seq_t) is a quadratic residue modulo p^e
   (p is an odd prime or 2).  Uses p-adic valuation and Legendre
   symbol for odd p; special rules for powers of 2.           */
static bool is_qr_mod_prime_power(seq_t n, uint64_t p, uint64_t e)
{
    /* n == 0 is always a square modulo any modulus. */
    if (n == 0) return true;

    seq_t tmp = n;
    uint64_t val = 0;          /* exponent of p in n */

    /* Extract the p-adic valuation of n. */
    while (tmp % (seq_t)p == 0) {
        ++val;
        tmp /= (seq_t)p;
    }

    if (val >= e) return true;  /* n ≡ 0 (mod p^e) */

    if (p == 2ULL) {            /* special case for 2^e */
        if (e == 1) return true;
        if (e == 2) {                /* modulo 4: residues 0,1 */
            seq_t m4 = n % 4ULL;
            return m4 == 0 || m4 == 1;
        }
        /* e >= 3 */
        if (val % 2 == 1) return false;   /* odd valuation */
        /* need (n / 2^val) ≡ 1 (mod 8) */
        uint64_t rem8 = ((uint64_t)tmp) % 8ULL;  /* tmp = n / 2^val */
        return rem8 == 1;
    }

    /* p is odd prime */
    if (val % 2 == 1) return false;           /* odd valuation */

    /* Now tmp = n / p^val is coprime to p.
       n is a quadratic residue mod p^e iff tmp is
       a quadratic residue mod p.                     */
    seq_t rem_p = tmp % (seq_t)p;
    uint64_t leg = mod_pow_uint64((uint64_t)rem_p, (p - 1) / 2, p);
    return leg == 1;
}

/* Check whether n is a quadratic residue modulo k.
   Factorises k and uses is_qr_mod_prime_power for each prime power. */
static bool has_square_root_mod_k(seq_t n, idx_t k)
{
    if (n == 0) return true;   /* 0 is always a square mod k */

    uint64_t kk = k;

    /* Handle factor 2 separately */
    if (kk % 2ULL == 0) {
        uint64_t e2 = 0;
        while (kk % 2ULL == 0) {
            ++e2;
            kk /= 2ULL;
        }
        if (!is_qr_mod_prime_power(n, 2ULL, e2)) return false;
    }

    /* Odd prime factors */
    for (uint64_t p = 3ULL; p <= kk / p; p += 2ULL) {
        if (kk % p == 0) {
            uint64_t e = 0;
            while (kk % p == 0) {
                ++e;
                kk /= p;
            }
            if (!is_qr_mod_prime_power(n, p, e)) return false;
        }
    }

    /* Remaining prime factor, if any */
    if (kk > 1ULL) {
        if (!is_qr_mod_prime_power(n, kk, 1ULL)) return false;
    }

    return true;
}

/* ------------------------------------------------------------ */
/* required OEIS sequence function */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    for (idx_t k = n + 1; ; ++k) {
        if (has_square_root_mod_k(n_seq, k))
            return (seq_t)k;          /* smallest k > n found */
    }
}
