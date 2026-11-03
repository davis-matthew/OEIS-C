/*
 * A167473  Primes p such that (p reversed)+10 is also a prime.
 * https://oeis.org/A167473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that reverse(p)+10 is prime
 * Range:     u8 n <= 9, u16 n <= 370, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 962 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_digits_idx.h"
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
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
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t len = 0;        /* number of terms stored */
    static size_t cap = 0;        /* capacity of cache */
    static idx_t cand = 2;        /* next candidate to test */

    /* grow cache capacity if needed */
    if (len >= n)            /* we already have the requested term */
        return cache[n-1];

    while (len < n) {
        /* find the next prime satisfying the condition */
        for (;;) {
            /* advance to next integer */
            cand++;
            if (cand == 2) continue;                /* 2 is not a candidate */
            if ((cand & 1) == 0) continue;          /* skip even numbers */

            if (!is_prime_big(cand)) continue;      /* not prime */

            /* reverse the digits and add 10 */
            seq_t revp   = reverse_digits_idx(cand);
            seq_t rev10  = revp + (seq_t)10;
            if (!is_prime_big(rev10)) continue;     /* fails condition */

            /* store the prime */
            if (len == cap) {
                size_t new_cap = cap ? cap << 1 : 64;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[len++] = (seq_t)cand;
            break;                                   /* next term found */
        }
    }

    return cache[n-1];
}
