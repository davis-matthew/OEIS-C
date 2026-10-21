/*
 * A099937  Primes not of the form floor(k + (1/2)*log(k)^2).
 * https://oeis.org/A099937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099937: Primes not of the form floor(k + (1/2)*log(k)^2)
   Formula: a(n) = the n-th prime p that is NOT equal to floor(k + 0.5*log(k)^2)
   for any integer k >= 1.
   This implementation precomputes all primes up to a fixed limit (2 000 000) and
   marks those that can be expressed as floor(k + 0.5*log(k)^2). The remaining
   primes are stored in a dynamically sized array. The function a(idx_t n)
   returns the n-th element of that array, with lazy initialisation that
   runs only on the first call. The algorithm is O(limit log log limit)
   for the sieve and O(limit) for marking the excluded primes, which
   satisfies the time constraints for n up to about 10 000. */

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t MAX_LIMIT = 2000000;        /* generous bound for the 10 kth term */
static bool   *is_prime = NULL;                /* sieve array */
static bool   *excluded = NULL;                /* indicates whether a prime is excluded */
static seq_t *good_primes = NULL;              /* primes that survive the test */
static size_t good_count = 0;                  /* number of primes in good_primes */
static bool initialized = false;

static void initialise(void)
{
    if (initialized)
        return;
    initialized = true;

    /* -------- 1. Sieve of Eratosthenes ----------------------------------- */
    is_prime = (bool*)malloc((MAX_LIMIT + 1) * sizeof(bool));
    excluded = (bool*)malloc((MAX_LIMIT + 1) * sizeof(bool));
    if (!is_prime || !excluded)
        return;                      /* memory failure, but unlikely for 2 MB */

    for (idx_t i = 0; i <= MAX_LIMIT; ++i) {
        is_prime[i] = true;
        excluded[i] = false;
    }
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; (idx_t)(p * p) <= MAX_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= MAX_LIMIT; m += p)
                is_prime[m] = false;
        }
    }

    /* -------- 2. Mark excluded primes ------------------------------------- */
    for (idx_t k = 1; k <= MAX_LIMIT; ++k) {
        double lk = log((double)k);
        double val_d = k + 0.5 * lk * lk;
        idx_t val = (idx_t)floor(val_d);
        if (val > MAX_LIMIT)
            break;
        if (is_prime[val])
            excluded[val] = true;
    }

    /* -------- 3. Build list of good primes -------------------------------- */
    /* Estimate how many primes we might keep; allocate a bit more. */
    good_primes = NULL;
    good_count  = 0;
    size_t good_cap = 0;

    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (is_prime[i] && !excluded[i]) {
            if (good_count == good_cap) {
                good_cap = good_cap == 0 ? 1024 : good_cap * 2;
                seq_t *new_buf = (seq_t*)realloc(good_primes, good_cap * sizeof(seq_t));
                if (!new_buf) break;          /* allocation failure */
                good_primes = new_buf;
            }
            good_primes[good_count++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)               /* out of bounds, not used by OEIS */
        return 0;

    initialise();

    if (n - 1 >= good_count) /* safety check */
        return 0;

    return good_primes[n - 1];
}
