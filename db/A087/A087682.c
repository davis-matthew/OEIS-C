/*
 * A087682  Numbers n such that n + 8 and n - 8 are both prime.
 * https://oeis.org/A087682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 878, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number n such that n-8 and n+8 are both prime.  This function
   generates the sequence A087682 by simple primality testing on
   incrementally accumulated primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

/* ---------- Prime table helpers ----------------------------------- */

static uint64_t *prime_list   = NULL;   /* storage for primes   */
static size_t   prime_count  = 0;       /* number of primes stored   */
static size_t   prime_cap    = 0;       /* allocated capacity       */
static uint64_t next_prime_candidate = 2;   /* next integer to test   */

/* Extend prime_list so it contains all primes ≤ limit.  The routine
   scans integers starting from next_prime_candidate, using the primes
   already known to do trial division.  It is very small because limit
   is at most the square root of the numbers being checked. */
static void generate_primes_up_to(uint64_t limit)
{
    if (limit < next_prime_candidate)
        return;

    for (uint64_t x = next_prime_candidate; x <= limit; ++x)
    {
        bool is_p = true;
        uint64_t r = (uint64_t)sqrt((double)x);
        for (size_t i = 0; i < prime_count && prime_list[i] <= r; ++i)
        {
            if (x % prime_list[i] == 0)
            {
                is_p = false;
                break;
            }
        }
        if (is_p)
        {
            if (prime_count == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                assert(prime_list);
            }
            prime_list[prime_count++] = x;
        }
    }
    next_prime_candidate = limit + 1;
}

/* Test whether n is prime using the incremental prime table.  */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1ULL) == 0)   return false;

    uint64_t r = (uint64_t)sqrt((double)n);
    generate_primes_up_to(r);
    for (size_t i = 0; i < prime_count && prime_list[i] <= r; ++i)
        if (n % prime_list[i] == 0)
            return false;
    return true;
}

/* ---------- Sequence storage ------------------------------------- */

static seq_t *terms   = NULL;   /* stored terms of the sequence */
static size_t terms_len = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* capacity of the array */
static uint64_t last_checked = 8; /* integer last examined for the condition */

/* ---------- Main function ---------------------------------------- */

seq_t a(idx_t n)
{
    /* initialise storage on first call */
    if (!terms)
    {
        terms_cap = 128;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        assert(terms);
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n)
    {
        ++last_checked;               /* next integer to test */

        /* n must be > 8 because n-8 must be ≥2.  We already start at 9,
           but we skip until condition holds. */
        if (last_checked <= 8) continue;

        if (is_prime_uint64(last_checked - 8) &&
            is_prime_uint64(last_checked + 8))
        {
            if (terms_len == terms_cap)
            {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                assert(terms);
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
