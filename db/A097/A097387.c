/*
 * A097387  Let f(x)=(largest digit of x)^(smallest digit of x) + x (A097385).
 *          Sequence gives numbers n such that f(n) and f(n+1) are both prime.
 * https://oeis.org/A097387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097387: numbers n for which f(n) and f(n+1) are prime,
    where f(x) = (largest digit of x)^(smallest digit of x) + x.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

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

/* return true iff seq_t number x satisfies f(x) prime and f(x+1) prime. */
static bool is_candidate(seq_t x)
{
    seq_t t = x;
    unsigned largest = 0, smallest = 9;

    if (t == 0) {
        largest = smallest = 0;
    } else {
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            t /= 10;
        }
    }

    seq_t fx = x + ipow((seq_t)largest, (seq_t)smallest);
    if (!is_prime_big(fx))
        return false;

    seq_t xn = x + 1;
    t = xn;
    largest = 0;
    smallest = 9;
    if (t == 0) {
        largest = smallest = 0;
    } else {
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d > largest) largest = d;
            if (d < smallest) smallest = d;
            t /= 10;
        }
    }
    seq_t fn = xn + ipow((seq_t)largest, (seq_t)smallest);
    return is_prime_big(fn);
}

/* Return the nth term of the sequence (a(1) is the first listed term). */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_start = 1;        /* next integer to test */

    /* initialise cache if needed */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = alloc_seq_array(cache_cap);
    }

    while (cache_len < n) {
        seq_t cand = (seq_t)next_start;

        if (is_candidate(cand)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
#if defined(__cplusplus)
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
#else
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_start;
    }

    return cache[n - 1];
}
