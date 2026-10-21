/*
 * A056039  Largest k such that (k!)^2 divides n!.
 * https://oeis.org/A056039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that (k!)^2 divides n!
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Uses binary search on k and Legendre's formula
   to test divisibility.  The search space is 1 … n‑1.
   For each candidate k a simple loop over the primes up to k
   checks the condition 2·exp_p(k!) ≤ exp_p(n!). */

#include <stddef.h>
#include <stdint.h>

#define MAX_PRIME 2000000                 /* upper bound for pre‑computed primes */

static unsigned char isComposite[MAX_PRIME + 1];
static int primes[160000];                /* enough for primes below MAX_PRIME */
static int primeCnt = 0;
static int primesReady = 0;

/* Pre‑compute all primes ≤ MAX_PRIME with a simple sieve. */
static void init_primes(void)
{
    if (primesReady) return;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!isComposite[i]) {
            primes[primeCnt++] = i;
            if ((long long)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    isComposite[j] = 1;
            }
        }
    }
    primesReady = 1;
}

/* Legendre’s formula: exponent of prime p in n! */
static int exp_in_factorial(uint64_t n, int p)
{
    int exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Check whether (k!)^2 divides n!.  The loop iterates over the
   primes up to k, so O(#primes ≤ k). */
static int fits(uint64_t k, uint64_t n)
{
    for (int i = 0; i < primeCnt; ++i) {
        int p = primes[i];
        if ((uint64_t)p > k) break;
        int k_exp = exp_in_factorial(k, p);
        int n_exp = exp_in_factorial(n, p);
        if ((2 * k_exp) > n_exp) return 0;
    }
    return 1;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;           /* a(1) = 1 */
    init_primes();

    uint64_t low  = 1;
    uint64_t high = (uint64_t)n - 1;        /* k < n */
    uint64_t best = 1;

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (fits(mid, (uint64_t)n)) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;           /* safety for unsigned wrap‑around */
            high = mid - 1;
        }
    }
    return (seq_t)best;
}
