/*
 * A140038  Primes of the form 24x^2+24xy+83y^2.
 * https://oeis.org/A140038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9633 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A140038 – primes of the form 24x² + 24xy + 83y².
 *
 *  A prime p occurs in the sequence iff
 *      p ≡ 83, 131, 227, 299, 395, 563, 635, 755,
 *        899, 923, 1091, 1139, 1403, 1427, 1811  (mod 1848).
 *
 *  The routine generates primes in ascending order by examining
 *  the 15 admissible residue classes modulo 1848. The search
 *  continues until the requested index n is reached, caching all
 *  intermediate terms so that subsequent calls are fast.
 *
 *  The code works for any unsigned integer type assigned to seq_t
 *  (uint8_t … uint64_t, unsigned __int128, or a bignum class in C++).
 *  It uses is_prime_big(seq_t) from the utils library for primality testing.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

 /* Admissible residues modulo 1848 – 15 values */
static const unsigned int residues[] = {
    83u, 131u, 227u, 299u, 395u, 563u,
    635u, 755u, 899u, 923u, 1091u, 1139u,
    1403u, 1427u, 1811u
};
#define RES_CNT   (sizeof(residues)/sizeof(residues[0]))
#define MODULO    1848u

/* Static state to allow memoisation across calls */
static bool          init_done   = false;
static idx_t         cur_k       = 0;   /* current block k      */
static size_t        cur_r       = 0;   /* current residue index */
static seq_t        *cache       = NULL; /* memoised terms       */
static size_t        cache_size  = 0;   /* how many terms cached*/
static size_t        cache_cap   = 0;   /* allocated capacity   */

/* Ensure the cache can hold up to 'need' terms. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp && new_cap > 0) return; /* allocation failure – caller will see 0 */
    cache       = tmp;
    cache_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined – keep safe */

    /* Initialise static state on entry. */
    if (!init_done) {
        init_done = true;
        cur_k     = 0;
        cur_r     = 0;
    }

    /* Grow cache if it currently cannot hold n terms. */
    ensure_cache_cap((size_t)n);

    /* Already computed? */
    if (cache_size >= (size_t)n) return cache[n - 1];

    /* Generate terms until we have the n‑th one. */
    while (cache_size < (size_t)n) {
        /* Candidate in the current residue class. */
        seq_t candidate = (seq_t)MODULO * (seq_t)cur_k + (seq_t)residues[cur_r];

        /* Test for primality. */
        if (is_prime_big(candidate))
            cache[cache_size++] = candidate;

        /* Advance to next residue class (or next block). */
        ++cur_r;
        if (cur_r == RES_CNT) {
            cur_r = 0;
            ++cur_k;
        }
    }

    return cache[n - 1];
}
