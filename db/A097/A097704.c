/*
 * A097704  Terms of A097703 that are not of the form 3*k + 1.
 * https://oeis.org/A097704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that (2*k+1) is divisible by a
 *            square * of a prime (i.e., 2*k+1 is not square‑free) and k ≢ 1
 *            (mod 3)
 * Range:     u8 n <= 7, u16 n <= 2587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with a(1)=12, a(2)=24, a(3)=60, … as in OEIS A097704. */

#include <stdlib.h>
#include <stdint.h>

/* Prime sieve up to a fixed limit (here 2000 is more than enough for the
 * expected range of k). */
#define PRIME_LIMIT 2000

static int *primes = NULL;        /* array of prime numbers */
static size_t prime_cnt = 0;      /* number of primes found */

/* Initialize the prime table once.  Called lazily from a(). */
static void init_primes(void)
{
    if (primes != NULL) return;           /* already done */

    int sieve[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (int p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (sieve[p]) {
            for (int m = p * p; m <= PRIME_LIMIT; m += p) sieve[m] = 0;
        }
    }

    /* count primes */
    for (int i = 2; i <= PRIME_LIMIT; ++i) if (sieve[i]) ++prime_cnt;

    primes = (int *)malloc(prime_cnt * sizeof(int));
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[idx++] = i;
}

/* Integer square root: floor(sqrt(x)).  Implemented by binary search
 * to stay within pure integer arithmetic. */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low = 0, high = (seq_t)1 << (sizeof(seq_t) * 4);
    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1);
        if (mid > 0 && mid > x / mid) {   /* mid*mid > x ?  compare to avoid overflow */
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

/* Cache of computed sequence values.  Cached values are stored in
 * ascending order of n. */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t next_k       = 1;      /* next candidate k to test */

/* Grow the cache if necessary. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();           /* out‑of‑memory */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Core function: returns the n‑th term of A097704.  Handles arbitrary
 * order of calls by memoizing results. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0; return 0 just in case */

    /* Lazy initialization of primes and cache */
    if (primes == NULL) init_primes();
    if (cache == NULL) {
        cache = (seq_t *)malloc(8 * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = 8;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t k = next_k++;
        /* Skip numbers of the form 3*k+1 */
        if ((k % 3) == 1) continue;

        seq_t m = (seq_t)2 * k + 1;                      /* 2*k+1 */
        seq_t sqrt_m = floor_sqrt_seq(m);
        int has_square = 0;
        for (size_t i = 0; i < prime_cnt && (seq_t)primes[i] <= sqrt_m; ++i) {
            seq_t p = primes[i];
            seq_t p2 = p * p;
            if (p2 > m) break;
            if (m % p2 == 0) { has_square = 1; break; }
        }
        if (has_square) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
