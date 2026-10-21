/*
 * A051935  a(n) = smallest number > a(n-1) such that a(1) + a(2) + ... + a(n)
 *          is a prime.
 * https://oeis.org/A051935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) is the smallest integer > a(n‑1) such that the cumulative
 *            sum ∑_{k=1..n} a(k) is prime
 * Range:     u8 n <= 56, u16 n <= 4391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the next term and caches all
   previously computed terms.  It uses a deterministic Miller‑Rabin
   test for 64‑bit integers to decide primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- modular multiplication and exponentiation with 128‑bit guard ---*/
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/*--- deterministic Miller–Rabin primality test for 64‑bit values ---*/
static inline bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint32_t small_primes[] =
        {2U,3U,5U,7U,11U,13U,17U,19U,23U,29U,31U,37U,41U,43U,47U};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/*--- cached state for the sequence ---*/
static seq_t *terms = NULL;    /* a(1), a(2), …          */
static idx_t *sums  = NULL;    /* cumulative sums       */
static idx_t  cache_size = 0;  /* number of terms stored */
static idx_t  cache_cap  = 0;  /* allocated capacity    */

/*--- ensure enough storage for at least 'need' terms ---*/
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    sums  = (idx_t *)realloc(sums,  new_cap * sizeof(idx_t));
    cache_cap = new_cap;
}

/*--- main function ---------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; the sequence is defined for n ≥ 1 */

    if (cache_size == 0) {                 /* first term lazily initialized */
        ensure_capacity(1);
        terms[0] = 2;            /* a(1) = 2 */
        sums[0]  = 2;            /* cumulative sum after first term */
        cache_size = 1;
    }

    /* return cached value if already computed */
    if (n <= cache_size) return terms[n-1];

    /* compute required terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t last = terms[cache_size-1];
        seq_t cand = last + 1;
        idx_t sum = sums[cache_size-1];

        /* search for the smallest cand such that sum + cand is prime */
        while (true) {
            idx_t s = sum + (idx_t)cand;
            if (is_prime_idx(s)) break;
            ++cand;
        }

        ensure_capacity(i);
        terms[cache_size] = cand;
        sums[cache_size]  = sum + (idx_t)cand;
        ++cache_size;
    }

    return terms[n-1];
}
