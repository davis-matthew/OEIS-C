/*
 * A057762  Numbers k such that 5 is a square mod k.
 * https://oeis.org/A057762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 5 is a quadratic
 *            residue modulo k. Equivalently, k is not divisible by 3, 8, or
 *            25; the exponent of 2 in k is at most 2; the exponent of 5 in k
 *            is at most 1; and every prime factor of k larger than 5
 *            satisfies p ≡ ±1 (mod 5)
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------------*/
/*--- prime sieve -------------------------------------------------*/
#define PRIME_LIMIT 1000000   /* ≈sqrt(10⁴⁰) is more than enough for n≤10000 */

static uint32_t primes[78500];      /* number of primes below 1e6 ≈ 78498 */
static size_t  prime_count = 0;

static void build_primes(void)
{
    static int built = 0;
    if (built) return;
    /* simple sieve up to PRIME_LIMIT */
    static uint8_t is_comp[PRIME_LIMIT + 1];
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT)
            {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    built = 1;
}

/*------------------------------------------------------------------*/
/*--- test whether k satisfies the condition ---------------------*/
static int ok_candidate(seq_t n)
{
    seq_t m = n;                    /* work on a copy */

    if ((m % (seq_t)3) == 0) return 0;
    if ((m % (seq_t)8) == 0) return 0;
    if ((m % (seq_t)25) == 0) return 0;

    /* exponent of 2 must be ≤2 */
    size_t e2 = 0;
    while ((m % (seq_t)2) == 0)
    {
        ++e2;
        m /= (seq_t)2;
        if (e2 > 2) return 0;
    }

    /* exponent of 5 must be ≤1 */
    size_t e5 = 0;
    while ((m % (seq_t)5) == 0)
    {
        ++e5;
        m /= (seq_t)5;
        if (e5 > 1) return 0;
    }

    /* factor remaining part using small primes */
    build_primes();
    for (size_t i = 0; i < prime_count &&
            (seq_t)primes[i] * (seq_t)primes[i] <= m; ++i)
    {
        seq_t p = (seq_t)primes[i];
        if (p == (seq_t)2 || p == (seq_t)5) continue;
        if ((m % p) == 0)
        {
            /* p > 5 and must be ≡ ±1 mod 5 */
            if ((primes[i] % 5) != 1 && (primes[i] % 5) != 4) return 0;
            while ((m % p) == 0)
                m /= p;
        }
    }

    /* if a prime factor larger than PRIME_LIMIT remains */
    if (m > (seq_t)1)
    {
        if (m > (seq_t)5)
        {
            if ((m % (seq_t)5) != (seq_t)1 && (m % (seq_t)5) != (seq_t)4)
                return 0;
        }
        /* exponent of 5 is already checked; only prime >5 could be left */
    }

    return 1;
}

/*------------------------------------------------------------------*/
/*--- cache for already found terms -------------------------------*/
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t  last_checked = 1;   /* next integer to test in sequence */

/* ensure the cache can store at least one more element */
static void ensure_cache_cap(void)
{
    if (cache_cap == 0)
    {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    }
    else if (cache_size + 1 > cache_cap)
    {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/*------------------------------------------------------------------*/
/*--- requested function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* negative or zero index is undefined; return 0 as safety */
    if (n == 0) return 0;

    while (cache_size < (size_t)n)
    {
        seq_t cand = (seq_t)last_checked;
        if (ok_candidate(cand))
        {
            ensure_cache_cap();
            cache[cache_size++] = cand;
        }
        ++last_checked;
    }
    return cache[n - 1];
}
