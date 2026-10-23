/*
 * A175270  Numbers k such that the least number of squares that add up to k
 *          equals the least number of triangular numbers that add up to k.
 *          Equivalently, A002828(k) = A061336(k).
 * https://oeis.org/A175270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer k such that the minimal number of squares
 *            that add up to k equals the minimal number of triangular numbers
 *            that add up to k. The sequence starts with offset 1: a(1)=0. The
 *            minimal square representation uses Lagrange's theorem: * 1
 *            square if k is a perfect square, * 2 squares i...
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Integer square root for uint64_t ----------------------------------- */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* --- Prime table -------------------------------------------------------- */
#define PRIME_LIMIT 2000000U          /* enough for the required range   */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void init_primes(void)
{
    static bool once = false;
    if (once) return;
    once = true;

    uint64_t N = PRIME_LIMIT;
    uint8_t *is_prime = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!is_prime) return;             /* allocation failure unlikely */

    for (uint64_t i = 0; i <= N; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    uint64_t sqrtN = isqrt_u64(N);
    for (uint64_t p = 2; p <= sqrtN; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= N; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (uint64_t i = 2; i <= N; ++i)
        if (is_prime[i]) ++count;

    primes_cap = count;
    primes_len = 0;
    primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    for (uint64_t i = 2; i <= N; ++i)
        if (is_prime[i]) primes[primes_len++] = i;

    free(is_prime);
}

/* --- Test if n is representable as sum of two squares ------------------- */
/* Returns true if n can be expressed as a^2 + b^2. */
static bool sum_two_squares(uint64_t n)
{
    if (n == 0) return true;
    init_primes();

    uint64_t m = n;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) break;
        int cnt = 0;
        while ((m % p) == 0) {
            m /= p;
            ++cnt;
        }
        if ((p & 3) == 3 && (cnt & 1)) return false;
    }
    if ((m & 3) == 3) return false;
    return true;
}

/* --- Minimal number of squares needed to represent k --------------------- */
/* Returns 0 for k==0; else 1-4.  */
static int min_squares(uint64_t k)
{
    if (k == 0) return 0;

    uint64_t r = isqrt_u64(k);
    if (r * r == k) return 1;                 /* perfect square */

    if (sum_two_squares(k)) return 2;         /* sum of two squares */

    /* Check Legendre's condition for 4 squares */
    uint64_t t = k;
    while ((t & 3) == 0) t >>= 2;            /* divide by 4^a */
    if ((t & 7) == 7) return 4;              /* of the form 4^a(8b+7) */
    return 3;
}

/* --- Minimal number of triangular numbers needed to represent k ----------- */
static int min_triangular(uint64_t k)
{
    if (k == 0) return 0;

    /* Check if k is triangular: 8k+1 is a square */
    uint64_t t1 = 8 * k + 1;
    uint64_t r1 = isqrt_u64(t1);
    if (r1 * r1 == t1) return 1;

    /* Sum of two triangular numbers iff 8k+2 is a sum of two squares */
    uint64_t t2 = 8 * k + 2;
    if (sum_two_squares(t2)) return 2;

    return 3;                                 /* otherwise needs 3 */
}

/* --- Storage for precomputed sequence ----------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_k = 0;                    /* next integer to examine */

/* Ensure the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        uint64_t k = next_k;
        if (min_squares(k) == min_triangular(k)) {
            /* store k */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)k;
            ++cache_len;
        }
        ++next_k;
    }
}

/* Public function required by the testing harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_terms(n);
    return cache[n - 1];
}
