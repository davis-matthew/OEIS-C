/*
 * A072274  List of Ormiston prime pairs.
 * https://oeis.org/A072274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(2k-1) = k-th prime that has as its next prime a permutation *
 *            of its digits; a(2k) = nextprime(a(2k-1) + 1) * * The
 *            implementation repeatedly generates primes, checks for digit *
 *            permutation with the previous prime, and stores each found pair
 *            as * two consecutive terms of the sequence. A dynami...
 * Range:     u8 none, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts_uint64, same_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_counts_uint64.h"
#include "../utils/same_digits_uint64.h"

/* ------------------------------------------------------------------ */
/*  Deterministic Miller–Rabin for 64‑bit unsigned integers           */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul64(r, a, m);
        a = mod_mul64(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes trial division */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Miller–Rabin with 7 bases enough for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static uint64_t next_prime_uint64(uint64_t n)
{
    if (n < 2) return 2;
    if ((n & 1) == 0) n++;          /* start from next odd number */
    while (!is_prime_uint64(n)) n += 2;
    return n;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Cached sequence storage                                            */
static seq_t *terms = NULL;       /* dynamic array of values          */
static size_t terms_size = 0;     /* number of stored terms           */
static size_t terms_cap  = 0;     /* current capacity of the array    */

static uint64_t prev_prime = 0;   /* last prime examined              */
static bool have_prev = false;    /* whether prev_prime is set        */

/* ------------------------------------------------------------------ */
/*  Ensure the cached array has at least 'new_cap' entries             */
static void grow_terms(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t new_size = terms_cap ? terms_cap * 2 : 32;
    if (new_size < new_cap) new_size = new_cap;
    void *ptr = realloc(terms, new_size * sizeof(seq_t));
    if (!ptr) abort();
    terms = (seq_t *)ptr;
    terms_cap = new_size;
}

/* ------------------------------------------------------------------ */
/*  Generate primes and collect Ormiston pairs until at least 'n' terms */
static void ensure_terms(idx_t n)
{
    if (terms_size >= (size_t)n) return;          /* already enough */

    uint64_t cand = have_prev ? prev_prime + 1 : 2;

    while (terms_size < (size_t)n) {
        uint64_t p = next_prime_uint64(cand);

        if (have_prev && same_digits_uint64(prev_prime, p)) {
            grow_terms(terms_size + 2);
            terms[terms_size++] = (seq_t)prev_prime;
            terms[terms_size++] = (seq_t)p;
        }

        prev_prime = p;
        have_prev  = true;
        cand = p + 1;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of bounds */
    ensure_terms(n);
    return terms[n - 1];
}
