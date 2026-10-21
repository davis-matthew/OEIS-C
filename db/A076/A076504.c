/*
 * A076504  Numbers k such that (k-1, k+1) and (k/2-1, k/2+1) are both pairs
 *          of twin primes.
 * https://oeis.org/A076504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076504: 2*A066388(n)
   Numbers k such that (k-1, k+1) and (k/2-1, k/2+1) are twin primes.
   We generate them by testing even m with the twin‑prime conditions and
   returning k = 2*m.  A(n) = 2 * m.                                    */

#include <stdint.h>
#include <stddef.h>

/* ----- deterministic Miller–Rabin primality test for 64‑bit numbers ----- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

static int is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return 0;
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p)
            return 1;
        if (n % p == 0)
            return 0;
    }
    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1U) == 0U)
    {
        d >>= 1U;
        ++s;
    }
    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        int composite = 1;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
            {
                composite = 0;
                break;
            }
        }
        if (composite)
            return 0;
    }
    return 1;
}

/* ----- sequence storage ----- */
#define MAX_TERMS 20000
static seq_t seq_results[MAX_TERMS];      /* stored as the build's seq_t type */
static uint64_t  seq_values[MAX_TERMS];    /* 64‑bit intermediate value */
static size_t   seq_len = 0;              /* number of terms computed */
static uint64_t next_m  = 6;              /* next even m to test */

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        uint64_t m = next_m;   /* m is even */

        /* check twin‑prime conditions */
        if (is_prime_uint64(m - 1) &&
            is_prime_uint64(m + 1) &&
            is_prime_uint64(2 * m - 1) &&
            is_prime_uint64(2 * m + 1))
        {
            uint64_t k = 2 * m;
            seq_values[seq_len] = k;
            seq_results[seq_len] = (seq_t)k;
            ++seq_len;
        }
        next_m += 2;   /* advance to next even m */
    }

    return seq_results[n - 1];
}
