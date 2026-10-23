/*
 * A101681  Numbers k such that gcd(C(2k,k), 2k+1) > 1.
 * https://oeis.org/A101681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A101681
    a(n) = the n‑th positive integer k such that gcd( C(2k,k) , 2k+1 ) > 1
    (C(2k,k) is the central binomial coefficient).  */
#include <stdbool.h>
#include <stdint.h>

/* ---- Prime sieve --------------------------------------------------- */
#define PRIME_LIMIT 200000          /* enough for all k needed up to ~10 000 */
static int primes[PRIME_LIMIT];
static int num_primes = 0;
static bool primes_done = false;

static void sieve_primes(void)
{
    if (primes_done) return;
    static bool is_comp[PRIME_LIMIT];
    for (int i = 2; i < PRIME_LIMIT; ++i) is_comp[i] = false;
    for (int i = 2; i * i < PRIME_LIMIT; ++i)
        if (!is_comp[i])
            for (int j = i * i; j < PRIME_LIMIT; j += i)
                is_comp[j] = true;
    for (int i = 2; i < PRIME_LIMIT; ++i)
        if (!is_comp[i]) primes[num_primes++] = i;
    primes_done = true;
}

/* ---- Test whether C(2k,k) shares a prime factor with 2k+1 -------- */
static bool has_common_prime(uint64_t k)
{
    uint64_t d = 2 * k + 1;              /* divisor to test against */
    uint64_t rem = d;

    /* factor d using primes up to sqrt(rem) */
    for (int ip = 0; ip < num_primes &&
         (uint64_t)primes[ip] * primes[ip] <= rem; ++ip)
    {
        int p = primes[ip];
        if (rem % p) continue;

        /* compute exponent of p in C(2k,k) using Legendre's formula */
        uint64_t power = p;
        uint64_t exp = 0;
        while (power <= 2 * k)
        {
            uint64_t a = (2 * k) / power;
            uint64_t b = k / power;      /* floor(k/power) */
            exp += a - 2 * b;
            if (power > UINT64_MAX / (uint64_t)p) break;
            power *= p;
        }
        if (exp > 0) return true;          /* common divisor found */

        /* strip this prime from the remainder */
        while (rem % p == 0) rem /= p;
    }

    /* remaining part of rem (if any) is > sqrt(original d) and is prime */
    if (rem > 1)
    {
        uint64_t p = rem;
        uint64_t power = p;
        uint64_t exp = 0;
        while (power <= 2 * k)
        {
            uint64_t a = (2 * k) / power;
            uint64_t b = k / power;
            exp += a - 2 * b;
            if (power > UINT64_MAX / p) break;
            power *= p;
        }
        if (exp > 0) return true;
    }
    return false;
}

/* ---- Static storage for sequence values --------------------------- */
#define SEQ_CAP 2000000
static seq_t seqbuf[SEQ_CAP];
static idx_t seqsize = 0;               /* number of terms already found */
static uint64_t candidate = 1;           /* next k to test                */

/* ---- Public entry point ------------------------------------------- */
seq_t a(idx_t n)
{
    sieve_primes();                     /* ensure primes ready */

    /* generate terms until we have at least n of them */
    while (seqsize < (idx_t)n)
    {
        /* find the next k with gcd(C(2k,k),2k+1) > 1 */
        while (!has_common_prime(candidate))
            ++candidate;

        /* store the term */
        if (seqsize < SEQ_CAP)
            seqbuf[seqsize++] = (seq_t)candidate;
        else
            return 0;                    /* out of range, should not happen */

        ++candidate;
    }

    return seqbuf[n - 1];   /* a(1) is the first element of the sequence */
}
