/*
 * A229289  Primes p of the form p = 2^k * m + 1, where (i) m is squarefree
 *          and odd, (ii) all primes that divide m are in the sequence, and
 *          (iii) k is 0, 1, or 2.
 * https://oeis.org/A229289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS A229289: */
/* primes p = 2^k · m + 1 with k ∈ {0,1,2}, m odd square‑free, */
/* and every prime divisor of m already appears earlier in the sequence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Grow a seq_t array to at least 'need' elements. */
static void ensure_cap(size_t *cap, seq_t **arr, size_t need)
{
    if (need <= *cap) return;
    if (*cap == 0) *cap = 64;
    while (*cap < need) *cap <<= 1;
    void *tmp = realloc(*arr, *cap * sizeof(seq_t));
    if (tmp) *arr = (seq_t *)tmp;
}

/* Binary search: does 'arr[0..len-1]' contain 'x'?  */
static bool seq_contains(const seq_t *arr, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < x) lo = mid + 1;
        else if (arr[mid] > x) hi = mid;
        else return true;
    }
    return false;
}

/* Test whether 'x' is prime using the current list of primes. */
static bool is_prime(const seq_t *primes, size_t primes_len, seq_t x)
{
    if (x < 2) return false;
    seq_t sqrt_x = floor_sqrt(x);
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = primes[i];
        if (p > sqrt_x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Determine whether the prime 'p' satisfies the sequence definition. */
static bool qualifies(seq_t p, const seq_t *seq, size_t seq_len)
{
    seq_t t = p - 1;
    unsigned e = 0;
    while ((t & 1) == 0) {   /* count power of 2 */
        t >>= 1;
        ++e;
        if (e > 2) return false;   /* k must be ≤ 2 */
    }
    seq_t m = t;                   /* odd part */
    if (m == 1) return true;       /* m = 1 is allowed */

    seq_t temp = m;
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t q = seq[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            unsigned exp = 0;
            while (temp % q == 0) {
                temp /= q;
                ++exp;
                if (exp > 1) return false;   /* not squarefree */
            }
            if (!seq_contains(seq, seq_len, q)) return false;
        }
    }
    if (temp > 1) {   /* last prime factor > sqrt(m) */
        if (!seq_contains(seq, seq_len, temp)) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static state shared between calls */
    static seq_t *seq     = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *primes  = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static seq_t candidate = 3;   /* next odd number to test */

    /* Initialise on first call */
    if (seq_len == 0) {
        ensure_cap(&primes_cap, &primes, 1);
        primes[0] = 2;
        primes_len = 1;

        ensure_cap(&seq_cap, &seq, 1);
        seq[0] = 2;
        seq_len = 1;
    }

    /* Generate up to the desired index */
    while (seq_len < n) {
        if (is_prime(primes, primes_len, candidate)) {
            /* Store prime in the prime list */
            ensure_cap(&primes_cap, &primes, primes_len + 1);
            primes[primes_len++] = candidate;

            /* Test the sequence condition */
            if (qualifies(candidate, seq, seq_len)) {
                ensure_cap(&seq_cap, &seq, seq_len + 1);
                seq[seq_len++] = candidate;
            }
        }
        candidate += 2;   /* skip even numbers */
    }

    return seq[n - 1];
}
