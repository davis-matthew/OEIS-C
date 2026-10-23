/*
 * A191034  Primes p with Jacobi symbol (p|51) = 1.
 * https://oeis.org/A191034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the n-th prime p with Jacobi(p|51) = 1 (n is 1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Helper:  Legendre symbol (a|q)  for a odd prime q (q=3,17)  */
/* ------------------------------------------------------------ */
static int legendre(uint64_t a, uint64_t q)
{
    /* a % q != 0 assumed */
    uint64_t exp = (q - 1) >> 1;            /* (q-1)/2 */
    uint64_t base = a % q;
    uint64_t res = 1;
    while (exp)
    {
        if (exp & 1U)
            res = (res * base) % q;
        base = (base * base) % q;
        exp >>= 1U;
    }
    if (res == 1)
        return 1;
    if (res == q - 1)
        return -1;
    return 0; /* should not occur for prime q */
}

/*  Jacobi symbol (p|51) = (p|3)*(p|17)  (p must not divide 51)  */
static int jacobi_p_over_51(uint64_t p)
{
    if ((p % 3U) == 0U || (p % 17U) == 0U)
        return 0;
    int l3  = legendre(p, 3U);
    int l17 = legendre(p, 17U);
    return l3 * l17;          /* product of two ±1 results */
}

/* ------------------------------------------------------------ */
/*  Prime storage and dynamic generation                       */
/* ------------------------------------------------------------ */
static uint64_t *prime_list     = NULL;  /* all primes found so far */
static size_t   prime_len     = 0;
static size_t   prime_cap     = 0;

static uint64_t *qual_list     = NULL;  /* primes with Jacobi = 1 */
static size_t   qual_len      = 0;
static size_t   qual_cap      = 0;

static void grow_array(uint64_t **arr, size_t *len, size_t *cap, size_t needed)
{
    if (needed <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 64 : (*cap << 1);
    while (newcap < needed)
        newcap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE); /* allocation failure */
    *arr = tmp;
    *cap = newcap;
}

/*  Test if candidate is prime using primes already stored  */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2U)
        return false;
    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t q = prime_list[i];
        if (q * q > cand)
            break;
        if (cand % q == 0U)
            return false;
    }
    return true;
}

/*  Generate primes up to (approximately) the specified limit    */
static void generate_primes_up_to(uint64_t limit)
{
    uint64_t cand = (prime_len == 0) ? 2U : prime_list[prime_len - 1] + 1U;
    for (; cand <= limit; ++cand)
    {
        if (!is_prime_candidate(cand))
            continue;
        grow_array(&prime_list, &prime_len, &prime_cap, prime_len + 1);
        prime_list[prime_len++] = cand;
        if (cand != 2U && cand != 3U && cand != 17U)
        {
            int js = jacobi_p_over_51(cand);
            if (js == 1)
            {
                grow_array(&qual_list, &qual_len, &qual_cap, qual_len + 1);
                qual_list[qual_len++] = cand;
            }
        }
    }
}

/*  Ensure qual_len >= target.  Generate more primes as needed.  */
static void ensure_quality(idx_t target)
{
    if (qual_len >= (size_t)target)
        return;
    /* Rough estimate of needed prime limit: 2*target log(2*target)   */
    uint64_t limit = 200000U;          /* start with a modest limit */
    while (qual_len < (size_t)target)
    {
        generate_primes_up_to(limit);
        limit <<= 1U;   /* double the limit and try again */
    }
}

/* ------------------------------------------------------------ */
/*  Sequence accessor                                                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe */

    ensure_quality(n);

    /* safety check for insane n beyond what we can store */
    if (n > qual_len)
        return (seq_t)0;
    return (seq_t)qual_list[(size_t)n - 1U];
}
