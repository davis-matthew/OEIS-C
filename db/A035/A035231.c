/*
 * A035231  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          -46.
 * https://oeis.org/A035231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nth index n of nonzero terms of the product * Π_p
 *            (1-(Kronecker(m,p)+1)p^(-s)+Kronecker(m,p)p^(-2s))^(-1) * for m
 *            = -46. The term n occurs iff every prime p with *
 *            Kronecker(-46,p) = -1 divides n to an even power. * *
 *            Implementation detail: * - For every prime p, Kronecker(-46,p)
 *            equals the Legen...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* modular exponentiation (base^exp mod mod) for 64‑bit values */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* Determine whether odd prime p is inert for m = -46
 * (Kronecker(-46,p) == -1).  Returns true only for inert primes.
 * The result is false for p|46 (i.e. 2 or 23). */
static bool is_inert_prime(uint32_t p)
{
    if (p == 2 || p == 23)
        return false;                    /* symbol 0 */

    /* (-46 mod p)  where p does not divide 46 */
    uint32_t m25 = 46 % p;                /* 46 mod p */
    uint32_t mmodp = (m25 == 0) ? 0
                              : p - m25;    /* -46 mod p */

    /* Legendre symbol (-46|p) via Euler's criterion:
     *   ((-46|p) == 1)   if mmodp^((p-1)/2) % p == 1
     *   ((-46|p) == -1)  if ... == p-1
     *   ((-46|p) == 0)   if mmodp == 0   (handled above) */
    uint32_t leg = (uint32_t)modpow64(mmodp, (uint64_t)(p - 1) / 2ULL, p);
    return (leg == p - 1U);              /* true only for -1 */
}

/* Sieve state -------------------------------------------------- */
static uint32_t *spf_arr   = NULL; /* smallest prime factor, 1‑based */
static uint32_t *primes_arr = NULL; /* list of primes */
static bool *inert_arr = NULL;   /* inert flag for primes */
static uint32_t spf_size = 0;     /* size of spf_arr (max number sieved) */
static uint32_t primes_cap = 0;   /* capacity of primes_arr */
static uint32_t primes_count = 0; /* current number of primes */

/* Extend the sieve so that all numbers up to limit are sieved. */
static void extend_sieve(uint32_t limit)
{
    if (limit <= spf_size)
        return;

    uint32_t new_size = spf_size ? spf_size * 2 : 32;
    while (new_size < limit)
        new_size <<= 1;

    /* Resize spf array, keeping previous data */
    uint32_t *new_spf = (uint32_t *)realloc(spf_arr,
                                            (new_size + 1) * sizeof(uint32_t));
    spf_arr = new_spf;
    for (uint32_t i = spf_size + 1; i <= new_size; ++i)
        spf_arr[i] = 0;
    spf_size = new_size;

    /* Resize prime list if necessary */
    if (primes_cap < primes_count + 1000) {
        uint32_t new_cap = primes_cap ? primes_cap * 2 : 64;
        if (new_cap < primes_count + 1000)
            new_cap = primes_count + 1000;
        uint32_t *new_pr = (uint32_t *)realloc(primes_arr,
                                               new_cap * sizeof(uint32_t));
        primes_arr = new_pr;
        primes_cap = new_cap;
    }

    /* Resize inert array for primes */
    bool *new_inert = (bool *)realloc(inert_arr,
                                      (new_size + 1) * sizeof(bool));
    inert_arr = new_inert;
    for (uint32_t i = spf_size + 1; i <= new_size; ++i)
        inert_arr[i] = false;

    /* Linear sieve */
    for (uint32_t i = 2; i <= new_size; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = i;
            if (primes_count == primes_cap) {
                uint32_t new_cap = primes_cap ? primes_cap * 2 : 64;
                primes_arr = (uint32_t *)realloc(primes_arr,
                                                 new_cap * sizeof(uint32_t));
                primes_cap = new_cap;
            }
            primes_arr[primes_count++] = i;
            inert_arr[i] = is_inert_prime(i);
        }
        for (uint32_t j = 0; j < primes_count; ++j) {
            uint32_t p = primes_arr[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > new_size)
                break;
            spf_arr[prod] = p;
            if (p == spf_arr[i])
                break;
        }
    }
}

/* Check whether x (≤ current sieve limit) has no odd exponent of any
 * inert prime.  All other primes are unrestricted. */
static bool allowed_number(uint64_t x)
{
    while (x > 1) {
        uint32_t p = (uint32_t)spf_arr[(uint32_t)x];
        uint32_t exp = 0;
        do {
            ++exp;
            x /= p;
        } while ((x % p) == 0);
        if (inert_arr[p] && (exp & 1U))
            return false;
    }
    return true;
}

/* --- Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static storage of computed sequence */
    static seq_t *seq = NULL;
    static size_t seq_size = 0;
    static size_t seq_cap = 0;
    static uint64_t next_candidate = 1; /* next integer to test */

    /* initialise once */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_size = 0;
        next_candidate = 1;
    }

    /* grow until we have at least n terms */
    while (seq_size < n) {
        /* ensure sieve covers next_candidate */
        if (next_candidate > spf_size)
            extend_sieve((uint32_t)(next_candidate * 2));

        if (allowed_number(next_candidate)) {
            if (seq_size == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq,
                                       seq_cap * sizeof(seq_t));
            }
            seq[seq_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
