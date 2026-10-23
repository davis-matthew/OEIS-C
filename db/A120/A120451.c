/*
 * A120451  Number of ways a prime number p can be expressed as 2*(p1-p2) +
 *          3*p3, where p1, p2, p3 are primes or 1, less than or equal to p.
 * https://oeis.org/A120451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A120451: for the n‑th prime p (1‑based), count the number of
   ordered triples (p1,p2,p3) with each of p1,p2,p3 being either 1 or a
   prime ≤ p that satisfy
             p = 2*(p1 – p2) + 3*p3.
   The function returns the result as seq_t.  The implementation
   uses incremental sieving and difference tables and works for any
   build (fixed‑size unsigned or arbitrary‑precision seq_t) and both
   C11 and C++17.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑defined types in the build */
typedef uint64_t idx_t;   /* external linkage – assume this type is available */

/* Max prime limit – sufficient for first ~20000 primes (covers n≤10000). */
#define PRIMELIM 200000

/* Static data – shared across calls. */
static bool          initialized = false;
static uint32_t      *primes      = NULL;   /* primes up to PRIMELIM      */
static size_t        prime_cnt   = 0;        /* number of primes stored   */
static seq_t        *result      = NULL;   /* answers for each prime    */

static uint32_t      *values      = NULL;   /* allowed numbers (1 + primes) */
static size_t        values_size = 0;
static size_t        values_cap  = 0;

static uint32_t      *diffCount   = NULL;   /* ordered‑pair counts by difference */
static const size_t  offset      = PRIMELIM;/* offset to handle negative diff */

static size_t        processed   = 0;       /* primes already processed    */

/* --------------------------------------------------------------------- */
/* Simple sieve: fill primes[] with all primes ≤ PRIMELIM and set prime_cnt.  */
static void sieve_primes(void)
{
    bool *is_prime = (bool*)calloc(PRIMELIM + 1, sizeof(bool));
    for (size_t i = 2; i <= PRIMELIM; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIMELIM; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIMELIM; j += i)
                is_prime[j] = false;

    /* first count primes to allocate exactly */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIMELIM; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIMELIM; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Initialise all static data structures. */
static void init(void)
{
    sieve_primes();

    /* Allocate result array */
    result = (seq_t*)malloc(prime_cnt * sizeof(seq_t));

    /* Allocate difference table */
    diffCount = (uint32_t*)calloc(2 * PRIMELIM + 1, sizeof(uint32_t));

    /* Prepare values array: start with only 1. */
    values_cap = 16;
    values = (uint32_t*)malloc(values_cap * sizeof(uint32_t));
    values[0] = 1;
    values_size = 1;

    /* Difference 0 from pair (1,1). */
    diffCount[offset] = 1;

    processed = 0;
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Ensure that the first n primes are processed and result[n‑1] is defined. */
static void ensure_processed(idx_t n)
{
    if (!initialized) init();

    while (processed < n && processed < prime_cnt)
    {
        uint32_t p = primes[processed];   /* current prime */

        /* Update diffCount with pairs involving the new prime. */
        for (size_t i = 0; i < values_size; ++i)
        {
            int32_t d = (int32_t)p - (int32_t)values[i];
            diffCount[(size_t)(d + offset)]++;
            diffCount[(size_t)(-d + offset)]++;
        }
        diffCount[offset]++;               /* pair (p,p) */

        /* Append the new prime to the list of allowed numbers. */
        if (values_size == values_cap)
        {
            values_cap = values_cap * 2 + 1;
            values = (uint32_t*)realloc(values, values_cap * sizeof(uint32_t));
        }
        values[values_size++] = p;

        /* Count representations for this prime. */
        seq_t cnt = 0;
        for (size_t i = 0; i < values_size; ++i)
        {
            uint32_t v = values[i];
            int64_t numer = (int64_t)p - 3LL * (int64_t)v;
            if ((numer & 1LL) != 0) continue;              /* must be even */
            int64_t d = numer / 2;                         /* required difference */
            if (d < -(int64_t)offset || d > (int64_t)offset) continue;
            uint32_t m = diffCount[(size_t)(d + offset)];
            if (m) cnt += (seq_t)m;
        }
        result[processed] = cnt;
        ++processed;
    }
}

/* --------------------------------------------------------------------- */
/* Public function as required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized) init();
    if (n > (idx_t)prime_cnt) return 0;   /* safety: out of precomputed range */
    ensure_processed(n);
    return result[n - 1];
}
