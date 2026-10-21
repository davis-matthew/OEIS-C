/*
 * A206815  Position of n+pi(n) in the joint ranking of {j+pi(j)} and
 *          {k+(k+1)/log(k+1)}.
 * https://oeis.org/A206815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gval
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the position of n+π(n) in the joint ranking of
   {j+π(j)} and {k+(k+1)/log(k+1)} (OEIS A206815).
   The rank is simply n plus the number of k such that
   k + (k+1)/log(k+1) ≤ n+π(n).  The function builds or extends
   a sieve for π(n) lazily and uses a binary search to count the
   second sequence.  It works for any integer type supplied for
   seq_t and idx_t. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/gval.h"

/*--- sieve data ---------------------------------------------------*/
static uint8_t *prime_flag = NULL;   /* flag[i] == 1 if i is prime   */
static uint32_t *prime_prefix = NULL;/* prefix[i] == π(i)            */
static idx_t sieve_limit = 0;

/* Ensure a sieve covering all numbers ≤ limit.
   The sieve is rebuilt entirely when a larger limit is requested. */
static void ensure_sieve(idx_t limit)
{
    if (limit < 1) limit = 1;           /* need at least index 1  */
    if (limit <= sieve_limit) return;  /* already large enough    */

    size_t sz = (size_t)limit + 1; /* indices 0..limit          */

    uint8_t *flag   = (uint8_t *)malloc(sz);
    uint32_t *pref  = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!flag || !pref) abort();

    /* initialize */
    for (size_t i = 0; i < sz; ++i) flag[i] = 1;
    flag[0] = flag[1] = 0;

    /* classic sieve of Eratosthenes */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (flag[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                flag[m] = 0;
        }
    }

    /* prefix sums for π(n) */
    pref[0] = 0;
    for (size_t i = 1; i < sz; ++i)
        pref[i] = pref[i - 1] + (uint32_t)flag[i];

    /* replace old data */
    free(prime_flag);
    free(prime_prefix);
    prime_flag   = flag;
    prime_prefix = pref;
    sieve_limit  = limit;
}

/* Compute π(n), the number of primes ≤ n. */
static seq_t compute_pi(idx_t n)
{
    ensure_sieve(n);
    return (seq_t)prime_prefix[n];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n=0; return 0 as placeholder */

    seq_t pi_n = compute_pi(n);          /* π(n)                          */
    unsigned long long x_ll = (unsigned long long)n + (unsigned long long)pi_n;
    double x_d = (double)x_ll;          /* for comparison with g(k)      */

    /* Binary search for the largest k with g(k) ≤ x_d. */
    uint64_t low = 1;
    uint64_t high = (x_ll > 0) ? x_ll : 1; /* g(k) > k, so k ≤ x     */
    uint64_t count_g = 0;

    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        if (gval(mid) <= x_d) {
            count_g = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;       /* safety for overflow          */
            high = mid - 1;
        }
    }

    /* Position is n plus the number of second‑sequence elements ≤ x. */
    return (seq_t)n + (seq_t)count_g;
}
