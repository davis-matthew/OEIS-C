/*
 * A079779  a(n) = smallest prime > n*prime(n).
 * https://oeis.org/A079779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > n*prime(n)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = A151800(A033286(n)) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Prime sieve (internal) ---------- */
static uint64_t *prime_cache = NULL;   /* array of first few primes (uint64_t) */
static size_t     prime_cnt   = 0;     /* number of primes stored      */
static idx_t      prime_limit = 0;     /* highest sieved value         */

/* Estimate an upper bound for the nth prime.
   For n ≥ 6, p_n < n*(log n + log log n) < n*20 for n ≤ 10000. */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    return n * 20;
}

/* Build a list of all primes ≤ limit and store it in prime_cache. */
static void rebuild_primes(idx_t limit)
{
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) { exit(EXIT_FAILURE); }

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate prime array */
    uint64_t *new_cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_cache) { free(is_prime); exit(EXIT_FAILURE); }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_cache[pos++] = (uint64_t)i;

    free(is_prime);

    /* Replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* Ensure at least n primes are available in prime_cache. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n) return;
    idx_t limit = estimate_limit(n);
    while (true) {
        rebuild_primes(limit);
        if (prime_cnt >= n) break;
        limit *= 2;                      /* grow if needed */
    }
}

/* ---------- Prime test (fast) ---------- */
/* We reuse a simple deterministic Miller–Rabin for 64‑bit integers
   plus a trivial check for small values. It is sufficient for all
   values that can occur in this sequence. */
static bool is_prime_big(seq_t x)
{
    if (x < 2) return false;
    /* Small primes */
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if ((seq_t)small_primes[i] == x) return true;
        if ((seq_t)small_primes[i] > x) return false;
    }
    /* Miller–Rabin bases for 64‑bit numbers (deterministic) */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    seq_t d = x - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = bases[i] % x;
        if (a == 0) return true;
        seq_t t = d;
        seq_t cur = 1;
        seq_t base = a;
        while (t) {
            if (t & 1) cur = (seq_t)(((unsigned __int128)cur * base) % x);
            base = (seq_t)(((unsigned __int128)base * base) % x);
            t >>= 1;
        }
        if (cur == 1 || cur == x - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            cur = (seq_t)(((unsigned __int128)cur * cur) % x);
            if (cur == x - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */

    ensure_primes(n);                     /* guarantee prime(n) is available */

    seq_t pn = (seq_t)prime_cache[n - 1]; /* prime(n) */
    seq_t prod = (seq_t)n * pn;           /* n * prime(n) */
    seq_t cand = prod + 1;                /* start > n*prime(n) */

    while (!is_prime_big(cand))
        ++cand;

    return cand;
}
