/*
 * A090708  Primes whose decimal representation is a valid number in base 5
 *          and interpreted as such is again a prime.
 * https://oeis.org/A090708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 61, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   to_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes sequence A090708.
   a(n) = the nth prime p (n ≥ 1) such that
   – every decimal digit of p is in {0,1,2,3,4}, and
   – the number obtained by interpreting those digits in base 5
     is also prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/to_base5.h"

/* ------------------------------------------------------------------ */
/* Helper: deterministic Miller–Rabin primality test for 64‑bit n   */
/* ------------------------------------------------------------------ */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint64_t small_primes[]
        = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[]
        = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        /* compute a^d mod n */
        uint64_t x = 1;
        uint64_t cur = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (unsigned __int128)x * cur % n;
            cur = (unsigned __int128)cur * cur % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Check that all decimal digits of n are ≤ 4 (0‑4)                    */
/* ------------------------------------------------------------------ */
static bool allowed_digits(uint64_t n)
{
    if (n == 0) return true;
    while (n) {
        uint8_t d = n % 10;
        if (d >= 5) return false;
        n /= 10;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Convert a decimal number consisting only of digits 0‑4 to base‑5  */

/* ------------------------------------------------------------------ */
/* Static storage for memoised terms and next candidate              */
/* ------------------------------------------------------------------ */
static seq_t *cache        = NULL; /* array of terms */
static size_t cache_size   = 0;    /* number of terms stored   */
static size_t cache_alloc  = 0;    /* current allocated size   */
static uint64_t next_candidate = 2; /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_alloc) return;
    size_t cap = cache_alloc ? cache_alloc : 8;
    while (cap < needed) cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!newp) {
        /* allocation failed – abort */
        abort();
    }
    cache = newp;
    cache_alloc = cap;
}

/* ------------------------------------------------------------------ */
/* Generate the next term of the sequence and store it in cache       */
/* ------------------------------------------------------------------ */
static void generate_next(void)
{
    while (1) {
        uint64_t cand = next_candidate++;
        if (!is_prime_u64(cand)) continue;
        if (!allowed_digits(cand)) continue;
        uint64_t b5 = to_base5(cand);
        if (!is_prime_u64(b5)) continue;

        /* cand satisfies all conditions – add to cache */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = (seq_t)cand;
        return;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */
    while (cache_size < (size_t)n) generate_next();
    return cache[n - 1];
}
