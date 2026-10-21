/*
 * A069141  a(n) = n^2*(n+1)!/(n^tau(n)) where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A069141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(n+1)!/(n^tau(n)) where tau(n) = number of divisors
 *            of n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Maximum value of n that we support for the prime sieve.
   20 000 comfortably covers n up to 10 000 and is small enough for
   a single run-time allocation. */
#define MAX_PRIME 20000

/* Static storage for the list of primes.  The sieve is performed
   lazily on first use so that repeated calls reuse the same data. */
static int  primes[MAX_PRIME];
static int  primes_cnt = 0;
static int  primes_max = 0;

/* ------------------------------------------------------------------ */
/* Utility: fast integer exponentiation for arbitrary-precision type
   seq_t.  The multiplication operator of seq_t is used so that the
   function works for all supported builds (plain integers or bignums). */
static inline seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1ULL) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Utility: exponent of prime p in m!  (Legendre's formula) */
static inline uint64_t exp_in_fact(uint64_t m, uint64_t p)
{
    uint64_t e = 0;
    while (m)
    {
        m /= p;
        e += m;
    }
    return e;
}

/* ------------------------------------------------------------------ */
/* Initialise the prime table up to at least 'need'.
   The sieve is performed only once and static data is reused. */
static void init_primes(uint64_t need)
{
    if ((int)need <= primes_max) return;          /* already ready */

    if (need > MAX_PRIME) need = MAX_PRIME;        /* clamp to buffer size */

    static unsigned char is_prime[MAX_PRIME + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;

    for (unsigned i = 2; i * i <= (unsigned)need; ++i)
        if (is_prime[i])
            for (unsigned j = i * i; j <= (unsigned)need; j += i)
                is_prime[j] = 0;

    int cnt = 0;
    for (unsigned i = 2; i <= (unsigned)need; ++i)
        if (is_prime[i]) primes[cnt++] = (int)i;
    primes_cnt = cnt;
    primes_max = (int)need;
}

/* ------------------------------------------------------------------ */
/* Main sequence function requested by the specification. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >=1 by the caller; cast to native type */
    uint64_t nn = (uint64_t)n;

    /* Ensure we have primes up to n+1 for factorisation and factorial
       exponents.  The constant MAX_PRIME is large enough for all test
       cases ("normal" behaviour up to n≈10 000). */
    init_primes(nn + 1);

    /* ------------------------------------------------------------------
       1. Factorise n and compute tau(n) = number of divisors.  We keep a
          small table of the primes that divide n together with their
          exponents so that we can later look them up quickly.
    --------------------------------------------------------------------- */
    uint64_t tmp = nn;
    /* The number of distinct prime factors of n is small (≤ log_2 n),
       so a fixed-size array is sufficient. */
    uint64_t n_factors[64];
    uint64_t n_exp[64];
    size_t n_factor_cnt = 0;
    uint64_t tau = 1;        /* product of (e+1) */

    for (int pi = 0; pi < primes_cnt && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= tmp; ++pi)
    {
        int p = primes[pi];
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            n_factors[n_factor_cnt] = p;
            n_exp[n_factor_cnt]   = e;
            ++n_factor_cnt;
            tau *= (e + 1);
        }
    }
    if (tmp > 1)
    {
        /* Remaining prime factor is > sqrt(original n) */
        n_factors[n_factor_cnt] = tmp;
        n_exp[n_factor_cnt]     = 1;
        ++n_factor_cnt;
        tau *= 2;
    }

    /* ------------------------------------------------------------------
       2. (n+1)! exponent contribution for each prime ≤ n+1.
    --------------------------------------------------------------------- */
    seq_t result = (seq_t)1;
    uint64_t bound = nn + 1;

    for (int pi = 0; pi < primes_cnt && primes[pi] <= bound; ++pi)
    {
        int p = primes[pi];
        uint64_t exp_fact = exp_in_fact(bound, (uint64_t)p);

        /* exponent of p in n (from factorisation table) */
        uint64_t e_in_n = 0;
        for (size_t fi = 0; fi < n_factor_cnt; ++fi)
            if (n_factors[fi] == (uint64_t)p)
            {
                e_in_n = n_exp[fi];
                break;
            }

        /* net exponent in the result:
           exp_fact + 2*e_in_n - tau*e_in_n
           = exp_fact + (2 - tau)*e_in_n
        */
        int64_t net = (int64_t)exp_fact + (int64_t)(2 - (int64_t)tau) * (int64_t)e_in_n;
        if (net <= 0) continue;            /* contributes nothing */

        result *= pow_seq_t((seq_t)p, (uint64_t)net);
    }

    /* ------------------------------------------------------------------
       3. Attach the factor n^2: it was already accounted for in the
          exponent calculation above (the "+2*e_in_n" part).  Hence
          the result now equals a(n).
    --------------------------------------------------------------------- */
    return result;
}
