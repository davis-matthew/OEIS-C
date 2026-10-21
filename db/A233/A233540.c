/*
 * A233540  Primes p such that p+2, p+8, and p+12 are all prime.
 * https://oeis.org/A233540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 61, u32 n <= 2300, u64 n <= 2300,
 *            u128 n <= 2300, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sequential construction of A233540 by testing each prime p
   for the conditions p+2, p+8, and p+12 being prime.  The values are kept
   in a static cache so non‑sequential calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ---------- modular arithmetic for 64‑bit numbers ---------- */

static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1)
            result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return result;
}

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */
static bool is_prime64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                              17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    unsigned i;
    for (i = 0; i < (sizeof(small)/sizeof(small[0])); ++i) {
        if (n % small[i] == 0)
            return n == small[i];
    }
    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* test bases */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (i = 0; i < (sizeof(bases)/sizeof(bases[0])); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        unsigned r;
        for (r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- static cache of found terms ---------- */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_size = 0;    /* number of terms stored */
static uint64_t last_checked = 1;/* last integer already inspected */

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, defensive */

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        /* Find next prime candidate */
        do {
            ++last_checked;
        } while (!is_prime64(last_checked));

        uint64_t p = last_checked;

        /* Test the required offsets */
        if (is_prime64(p + 2ULL) &&
            is_prime64(p + 8ULL) &&
            is_prime64(p + 12ULL)) {

            /* Append to cache, expanding if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1U];
}
