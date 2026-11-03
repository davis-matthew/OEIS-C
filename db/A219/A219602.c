/*
 * A219602  Primes p such that p^2-2 is composite.
 * https://oeis.org/A219602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A219602: primes p such that p²‑2 is composite.
   Computed by incremental prime search (is_prime), testing p²‑2 with
   is_prime_big. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Ensure an array has room for at least 'needed' elements. */
#ifdef __cplusplus
template <class T>
static T *grow_to(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
        *cap = new_cap;
    }
    return *ptr;
}
#else
static void *grow_to(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return *ptr;
}
#endif

/* Return the n‑th prime p with p²‑2 composite. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */

    /* Static state for out‑of‑order calls. */
    static seq_t *found = NULL;
    static size_t found_len = 0, found_cap = 0;
    static idx_t candidate = 2;     /* next integer to test */

    /* Keep generating until we have enough terms. */
    while (found_len < (size_t)n) {
        if (is_prime(candidate)) {
            /* test whether p²‑2 is composite */
            seq_t sq = (seq_t)candidate * (seq_t)candidate;
            seq_t test = sq - (seq_t)2;
            if (!is_prime_big(test)) {
                grow_to(found_len + 1, &found_cap, OEISC_VPP(&found), sizeof(seq_t));
                found[found_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }

    return found[n - 1];
}
