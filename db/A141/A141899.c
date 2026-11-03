/*
 * A141899  Primes of the form 2*3*5*7*k + 97.
 * https://oeis.org/A141899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 210*k + 97 (k ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 785 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache = NULL;      /* array of primes   */
    static size_t cached = 0;        /* number stored    */
    static size_t cap = 0;           /* current capacity */
    static idx_t next_k = 0;         /* next value of k  */

    if (n == 0)  /* undefined; return 0 for safety */
        return 0;

    /* zero‑based index of the wanted term */
    size_t target = (size_t)(n) - 1;

    /* expand cache until we have enough primes */
    while (cached <= target) {
        /* candidate prime = 210*k + 97 */
        seq_t candidate = (seq_t)97 + ((seq_t)210 * (seq_t)next_k);

        /* test primality (works for all seq_t sizes) */
        if (is_prime_big(candidate)) {
            if (cached >= cap) {
                /* grow capacity */
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                if (new_cap <= cached)
                    new_cap = cached + 1;
                seq_t *fresh = seq_realloc(cache, cap, new_cap);
                cache = fresh;
                cap = new_cap;
            }
            cache[cached++] = candidate;
        }
        next_k++;          /* advance to the next k */
    }

    return cache[target];
}
