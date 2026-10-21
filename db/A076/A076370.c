/*
 * A076370  a(n) is the smallest number k such that primorial(k)^2 >
 *          primorial(n).
 * https://oeis.org/A076370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 458, u16 n <= 9010, u32 n <= 9010, u64 n <= 9010,
 *            u128 n <= 9010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076370: a(n) is the smallest k such that primorial(k)^2 > primorial(n).
 * The function computes this by comparing logarithmic sums: 2·logP[k] > logP[n],
 * where logP[i] is the cumulative sum of log(prime_j) for j < i.
 * No table of terms is hard‑coded; primes are generated on demand via trial division.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ----------  static state  ---------- */
static size_t prime_count   = 0;   /* number of primes stored */
static size_t prime_cap     = 0;   /* allocated size for primes[]   */
static unsigned *primes     = NULL;/* array of primes */

static size_t log_cap       = 0;   /* allocated size for logP[]    */
static double *logP        = NULL; /* cumulative logarithms, logP[0] = 0 */

/* ----------  helper functions  ---------- */

/* ensure that the primes[] array can hold at least 'needed' primes */
static void ensure_prime_capacity(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 128;
    if (new_cap < needed) new_cap = needed;
    unsigned *tmp = (unsigned *)realloc(primes, new_cap * sizeof(unsigned));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    prime_cap = new_cap;
}

/* ensure that the logP[] array can hold at least 'needed' entries    */
static void ensure_log_capacity(size_t needed)
{
    if (log_cap >= needed) return;
    size_t new_cap = log_cap ? log_cap << 1 : 128;
    if (new_cap < needed) new_cap = needed;
    double *tmp = (double *)realloc(logP, new_cap * sizeof(double));
    if (!tmp) exit(EXIT_FAILURE);
    logP = tmp;
    log_cap = new_cap;
}

/* Generate primes until we have at least 'need' primes stored. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need) return;

    /* initialise storage if this is the first call */
    if (prime_count == 0) {
        prime_cap = 128;
        primes = (unsigned *)malloc(prime_cap * sizeof(unsigned));
        if (!primes) exit(EXIT_FAILURE);

        log_cap = 1;                 /* logP[0] will be filled later */
        logP   = (double *)malloc(log_cap * sizeof(double));
        if (!logP) exit(EXIT_FAILURE);
        logP[0] = 0.0;               /* primorial(0) = 1 */
    }

    unsigned cand = (prime_count == 0) ? 2 : primes[prime_count - 1] + 1;

    while (prime_count < need) {
        /* test whether cand is prime */
        bool isprime = true;
        unsigned sqrt_c = (unsigned)sqrt((double)cand);
        for (size_t i = 0; i < prime_count && primes[i] <= sqrt_c; ++i) {
            if (cand % primes[i] == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* store the new prime */
            ensure_prime_capacity(prime_count + 1);
            primes[prime_count] = cand;

            /* compute its cumulative logarithm */
            ensure_log_capacity(prime_count + 2);   /* need index (prime_count+1) */
            logP[prime_count + 1] = logP[prime_count] + log((double)cand);

            ++prime_count;
        }
        ++cand;
    }
}

/* ----------  main sequence function  ---------- */

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit index.  We need at least n primes.      */
    size_t need = (size_t) ((n > 0) ? n : 1);
    ensure_primes(need);       /* guarantees logP[n] exists */

    /* target = logP[n] / 2.  We seek minimal k with logP[k] > target.  */
    double target = logP[(size_t)n] * 0.5;

    size_t lo = 0;
    size_t hi = prime_count;   /* logP[prime_count] exists and is > target for n≥1 */

    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (logP[mid] > target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
