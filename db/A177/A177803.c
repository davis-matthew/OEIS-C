/*
 * A177803  The number of lines in the analog of Pratt primality certificate
 *          for the n-th semiprime.
 * https://oeis.org/A177803
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k semiprime, k | (n-1)} a(k); a(4) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (Recurrence uses only previously computed values; the sequence
    is defined for every n >= 4.) */

#include <stdint.h>
#include <stdbool.h>

/* Upper bound for precomputation: sufficient for n up to about 10 000. */
#define MAX_N 20000

/* Types supplied by the build: seq_t, idx_t.                           */
static bool     initialized = false;
static idx_t    spf[MAX_N + 1];                /* smallest prime factor   */
static seq_t   cache[MAX_N + 1];               /* memoized a(n), 0=unknown */

/* Simple linear sieve to fill SPF table.  Complexity O(MAX_N).          */
static void init_once(void)
{
    if (initialized) return;

    /* Mark primes and smallest prime factors. */
    for (idx_t i = 0; i <= MAX_N; ++i) spf[i] = 0;
    spf[0] = 0;  spf[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i)
    {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= MAX_N) {
                for (idx_t j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Base value: a(4) = 1.  All other entries remain 0 (unknown). */
    cache[4] = (seq_t)1;

    initialized = true;
}

/* Recursive, memoised computation of a(n).  Assumes n >= 4.           */
static seq_t a_rec(idx_t n)
{
    /* Recursive base: already cached. */
    if (cache[n] != 0) return cache[n];

    /* Factor n-1 using the SPF table.  Since n-1 <= MAX_N, this is safe. */
    idx_t m = n - 1;
    idx_t primes[16];   /* enough for m <= 20000 */
    idx_t exps[16];
    int   pcnt = 0;

    idx_t tmp = m;
    while (tmp > 1 && pcnt < 16) {
        idx_t p = spf[tmp];
        int   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* Sum a(k) over all semiprime divisors k of n-1. */
    seq_t sum = (seq_t)0;

    /* Case 1: square of a prime. */
    for (int i = 0; i < pcnt; ++i) {
        if (exps[i] >= 2) {
            idx_t d = primes[i] * primes[i];
            sum += a_rec(d);
        }
    }

    /* Case 2: product of two distinct primes. */
    for (int i = 0; i < pcnt; ++i) {
        for (int j = i + 1; j < pcnt; ++j) {
            idx_t d = primes[i] * primes[j];
            sum += a_rec(d);
        }
    }

    /* Final result. */
    seq_t result = (seq_t)1 + sum;
    cache[n] = result;
    return result;
}

/* Public interface required by the specification. */
seq_t a(idx_t n)
{
    /* We only care about n >= 4; lower values are undefined but
       return 0 in that case.  The specification says behaviour below
       the offset does not matter. */
    if (n < 4) return (seq_t)0;

    /* Ensure tables are ready. */
    init_once();

    /* If n exceeds preallocated range, fall back to a direct recursive
       evaluation without caching (still correct, though potentially slower). */
    if (n <= MAX_N) return a_rec(n);

    /* For n > MAX_N, perform recursion without using the cache. */
    idx_t m = n - 1;
    idx_t primes[16];
    idx_t exps[16];
    int   pcnt = 0;

    idx_t tmp = m;
    while (tmp > 1 && pcnt < 16) {
        idx_t p = spf[tmp];
        int   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }

    seq_t sum = (seq_t)0;
    for (int i = 0; i < pcnt; ++i) {
        if (exps[i] >= 2) sum += a(primes[i] * primes[i]);
    }
    for (int i = 0; i < pcnt; ++i) {
        for (int j = i + 1; j < pcnt; ++j) {
            sum += a(primes[i] * primes[j]);
        }
    }

    return (seq_t)1 + sum;
}
