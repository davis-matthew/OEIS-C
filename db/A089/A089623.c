/*
 * A089623  Numbers n such that n^2 + 2n - 1 is prime.
 * https://oeis.org/A089623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth integer n such that n^2 + 2n - 1 is prime.
 *         Equivalently, (n + 1)^2 - 2 is prime.
 * The sequence starts a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 6, ….
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------- deterministic 64‑bit Miller–Rabin primality test ------------- */
static int is_prime_u64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ull,3ull,5ull,7ull,11ull,13ull,17ull,19ull,23ull,29ull,31ull,37ull
    };
    for (size_t i = 0; i < sizeof small_primes/sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1u) == 0) {
        d >>= 1; ++s;
    }

    /* bases for deterministic test up to 2^64; see deterministic Miller–Rabin tables */
    static const uint64_t bases[] = {
        2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull
    };

    for (size_t i = 0; i < sizeof bases/sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __int128 x = 1, p = a % n, e = d;
        while (e > 0) {
            if (e & 1u) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1) continue;
        int composite = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            xx = (uint64_t)x;
            if (xx == n - 1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* --------------- sequence cache and generator --------------------------- */
static seq_t *term_cache = NULL;      /* dynamic array of sequence terms   */
static size_t  cache_len  = 0;         /* number of stored terms            */
static size_t  cache_cap  = 0;         /* current capacity of term_cache    */
static idx_t   next_candidate = 1;     /* next integer to test for primality */

/* helper to grow the cache array */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *new_ptr = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort(); /* out of memory */
    term_cache = new_ptr;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset 1; values below offset are undefined */

    /* compute terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_candidate;
        __int128 tmp = (__int128)(cand + 1) * (cand + 1) - 2; /* (n+1)^2 - 2 */
        uint64_t val = (uint64_t)tmp; /* fits in 64‑bit for used range */

        if (is_prime_u64(val)) {
            if (cache_len == cache_cap) grow_cache();
            term_cache[cache_len++] = (seq_t)cand;
        }
        ++next_candidate;
    }
    return term_cache[n - 1];
}
