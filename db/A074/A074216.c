/*
 * A074216  Squares m satisfying sigma(m) == 0 (mod 3).
 * https://oeis.org/A074216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A074216 – Squares m such that σ(m) ≡ 0 (mod 3)
 *  Property:  m is a perfect square and for
 *      m = ∏p_i^(2e_i)    there exists a prime p_i ≡ 1 (mod 3)
 *  with e_i ≡ 1 (mod 3).
 *  Equivalently, let x = √m. Then x must have at least one prime factor
 *  p ≡ 1 (mod 3) with exponent e ≡ 1 (mod 3).
 *  The sequence is a(n) = (A072864(n))^2.
 *  Implementation:  generate integers x one by one, test the above
 *  condition by trial division using a small prime table (≤ 200 000),
 *  and store their squares.  The cache is expanded lazily so that
 *  repeated calls with arbitrary indices are efficient.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Prime table (sieved up to 200000) -------------------- */
#define PRIME_LIMIT 200000
static uint32_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static int primes_built = 0;

static void build_primes(void)
{
    if (primes_built) return;
    int *is_comp = (int*)calloc(PRIME_LIMIT + 1, sizeof(int));
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count < PRIME_LIMIT)
                primes[prime_count++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_built = 1;
}

/* ---------- Cache for the sequence (dynamic array) ---------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;  /* number of terms stored */
static size_t cache_cap  = 0;
static uint32_t next_candidate = 1;  /* next integer to test (x) */

/* Ensure cache has capacity at least 'need' */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* malloc failed – abort */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Test whether integer x satisfies the prime‑exponent condition.  */
static int ok_x(uint32_t x)
{
    uint32_t tmp = x;
    for (size_t pi = 0; pi < prime_count && (uint32_t)primes[pi] * primes[pi] <= tmp; ++pi) {
        uint32_t p = primes[pi];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if ((p % 3 == 1) && (e % 3 == 1))
                return 1;  /* condition satisfied */
        }
    }
    /* Remaining factor > 1 is prime */
    if (tmp > 1 && (tmp % 3 == 1))
        return 1;
    return 0;
}

/* Generate terms until we have at least 'n' values. */
static void generate_to_n(idx_t n)
{
    build_primes();   /* make sure primes are ready */

    while (cache_size < (size_t)n) {
        ++next_candidate;         /* test the next integer x */

        if (ok_x(next_candidate)) {
            seq_t x_sq = (seq_t)next_candidate;
            x_sq = x_sq * x_sq;      /* compute square */
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = x_sq;
        }
    }
}

/* --------------------- Public interface --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined, but return 0 for safety */
        return (seq_t)0;
    }
    generate_to_n(n);
    return cache[n - 1];
}
