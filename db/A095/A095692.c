/*
 * A095692  Primes of the form n^3 + n + 1.
 * https://oeis.org/A095692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form m^3 + m + 1 (m >= 1)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 269, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* small primes for fast trial division */
static const uint32_t small_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97
};
static const size_t num_small_primes = sizeof(small_primes)/sizeof(small_primes[0]);

/* deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < num_small_primes; ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases valid for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, pow = a, exp = d;
        while (exp) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* dynamic storage of found primes */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;
static uint64_t last_m   = 0;  /* m of last prime found */

/* ensure capacity of the array */
static void ensure_terms_capacity(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset is 1, treat 0 as undefined */
    if ((size_t)n <= terms_size)   /* already computed term */
        return terms[n-1];

    /* extend the sequence until n terms are known */
    ensure_terms_capacity((size_t)n);
    for (uint64_t m = last_m + 1;; ++m) {
        __int128 mm = m;
        __int128 val128 = mm * mm * mm + mm + 1;   /* m^3 + m + 1 */
        uint64_t val = (uint64_t)val128;           /* fits 64‑bit for required size */
        if (isPrime64(val)) {
            terms[terms_size++] = (seq_t)val;
            last_m = m;
            if (terms_size == (size_t)n)
                return terms[n-1];
        }
        /* loop continues until enough terms are found */
    }
}
