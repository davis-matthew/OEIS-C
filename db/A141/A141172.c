/*
 * A141172  Primes of the form 2*x^2+2*x*y-3*y^2 (as well as of the form
 *          2*x^2+6*x*y+y^2).
 * https://oeis.org/A141172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1623, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9520 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 2*x^2+2*x*y-3*y^2
 * (equivalently, primes ≡ 1, 9, 25 (mod 28), plus the prime 2). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *set_cache = NULL;           /* cached primes */
static size_t set_cap   = 0;              /* allocated capacity */
static size_t set_size  = 0;              /* number of cached primes */
static idx_t  next_candidate = 3;         /* next odd number to test */

static void ensure_set_cache(idx_t n)
{
    if (n == 0)
        return;
    if (set_size >= (size_t)n)
        return;

    /* Allocate or grow cache to hold at least n elements. */
    if (set_cap < (size_t)n) {
        seq_t *tmp = seq_realloc(set_cache, set_cap, (size_t)n);
        if (!tmp)
            abort();
        set_cache = tmp;
        set_cap   = (size_t)n;
    }

    /* Insert the first prime if the cache is empty. */
    if (set_size == 0) {
        set_cache[set_size++] = (seq_t)2;
        next_candidate = 3;
    }

    /* Largest value representable in seq_t. */
    seq_t max_val = ~((seq_t)0);
    idx_t max_allowed = (idx_t)max_val;

    /* Search for remaining primes until the cache holds n primes. */
    while (set_size < (size_t)n) {
        if (next_candidate > max_allowed)
            abort();   /* further primes do not fit in seq_t */

        bool ok = false;
        idx_t r = next_candidate % 28ULL;
        if (next_candidate == 2 || r == 1 || r == 9 || r == 25)
            ok = true;

        if (ok && is_prime_big((seq_t)next_candidate))
            set_cache[set_size++] = (seq_t)next_candidate;

        next_candidate += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0 */
    ensure_set_cache(n);
    return set_cache[n - 1];
}
