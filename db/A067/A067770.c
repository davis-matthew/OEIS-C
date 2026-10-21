/*
 * A067770  a(n) = Catalan(n) mod (n+2).
 * https://oeis.org/A067770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) mod (n+2)
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime sieve constants – enough to cover n up to 10000 (2n ≤ 20000) */
#define PRIME_LIMIT 20000

static bool  is_comp[PRIME_LIMIT + 1];          /* composite flags */
static uint32_t primes[PRIME_LIMIT + 1];        /* prime list        */
static size_t  prime_cnt   = 0;                 /* number of primes  */
static bool    primes_inited = false;          /* init flag         */

/* Initialise the prime list once */
static void init_primes(void)
{
    if (primes_inited)
        return;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
            {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_inited = true;
}

/* 64‑bit modular multiplication – uses 128‑bit if available */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)((unsigned __int128)a * b % mod);
#else
    uint64_t res = 0;
    a %= mod;
    while (b)
    {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
#endif
}

/* 64‑bit modular exponentiation – exp fits in int (≤ n ≤ 10000) */
static uint64_t powmod_uint64(uint64_t base, int exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Legendre exponent of prime p in N! */
static int legendre(uint64_t N, uint32_t p)
{
    int e = 0;
    while (N)
    {
        N /= p;
        e += N;
    }
    return e;
}

/* Catalan(n) mod (n+2) – computed via prime‑factorisation of
   binomial(2n, n) / (n+1) and reducing modulo (n+2) along the way */
seq_t a(idx_t n)
{
    /* Handle the trivial low‑n cases directly */
    if (n < 2)
        return (seq_t)((seq_t)1 % ((seq_t)n + 2));

    init_primes();

    uint64_t mod     = (uint64_t)n + 2;            /* modulus M = n+2   */
    uint64_t result  = 1 % mod;                   /* product modulo M */
    uint64_t denom   = (uint64_t)n + 1;           /* divide by (n+1)   */
    uint64_t two_n   = (uint64_t)2 * n;
    uint64_t n_val   = (uint64_t)n;

    for (size_t idx = 0; idx < prime_cnt; ++idx)
    {
        uint32_t p = primes[idx];
        if ((uint64_t)p > two_n)
            break;

        int exp = legendre(two_n, p) - 2 * legendre(n_val, p);

        /* Subtract the exponent of p in (n+1) */
        uint64_t tmp = denom;
        while (tmp % p == 0)
        {
            exp--;
            tmp /= p;
        }

        if (exp <= 0)
            continue;

        /* multiply result by p^exp modulo mod */
        result = mul_mod64(result,
                           powmod_uint64((uint64_t)p, exp, mod),
                           mod);
    }

    return (seq_t)result;
}
