/*
 * A191071  Primes p that have Kronecker symbol (p|46) = -1.
 * https://oeis.org/A191071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p for which the Jacobi symbol (p|46) =
 *            -1. This uses the fact that (p|46) = (p|2)*(p|23) for p odd and
 *            p∤46
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   legendre_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/legendre_2.h"

/* ---------- static state ---------- */
static seq_t *seq_cache  = NULL;   /* cache of valid primes  */
static size_t seq_cap   = 0;       /* usable slots in seq_cache */
static size_t seq_len   = 0;       /* number of cached elements */

static uint64_t *prime_list = NULL;  /* list of all primes found so far */
static size_t prime_cap     = 0;      /* usable slots in prime_list */
static size_t prime_len     = 0;      /* number of cached primes */

static uint64_t candidate = 3;         /* next odd number to test */

/* ---------- helpers ---------- */

/* simple integer square‑root, floor(sqrt(n)) */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

/* Precompute quadratic residues modulo 23 once. */
static bool resid23[23] = {false};
static bool resid23_init = false;
static void init_resid23(void)
{
    if (resid23_init) return;
    resid23_init = true;
    for (int i = 0; i < 23; ++i) resid23[i] = false;
    for (int i = 0; i < 23; ++i) resid23[(i * i) % 23] = true;
}

/* Legendre symbol (p|23) for odd p≠23. */
static int legendre_23(uint64_t p)
{
    return resid23[p % 23] ? 1 : -1;
}

/* Append prime to prime list; if it satisfies the condition, store in cache. */
static void add_prime(uint64_t p)
{
    /* grow prime list if needed */
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;

    /* p=2 or p=23 gives symbol 0, not -1 */
    if (p == 2 || p == 23) return;

    int s2  = legendre_2(p);
    int s23 = legendre_23(p);
    if (s2 != s23) {                  /* (p|2)*(p|23)==-1 */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq_cache = tmp;
            seq_cap   = new_cap;
        }
        seq_cache[seq_len++] = (seq_t)p;   /* implicit conversion; assumed available */
    }
}

/* Basic primality test using previously found primes */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    uint64_t limit = isqrt_u64(x);
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t q = prime_list[i];
        if (q > limit) break;
        if (x % q == 0) return false;
    }
    return true;
}

/* Generate primes and cached sequence values until we have at least n entries. */
static void generate_upto(idx_t n)
{
    init_resid23();
    if (prime_len == 0) add_prime(2);          /* seed with first prime */

    while (seq_len < n) {
        if (is_prime(candidate))
            add_prime(candidate);
        candidate += 2;                /* only odd numbers */
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* below offset – undefined */
    generate_upto(n);
    return seq_cache[n - 1];
}
