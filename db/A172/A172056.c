/*
 * A172056  Primes p such that 2*p+-1 and 2*p+-3 are all composites.
 * https://oeis.org/A172056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 2p-3, 2p-1, 2p+1, and 2p+3 are
 *            all composite
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 787 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
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

seq_t a(idx_t n)
{
    /* static caching to handle out-of-order calls */
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;
    static idx_t last_checked = 1;

    /* Ensure cache is large enough for the requested index */
    while (sz < (size_t)n)
    {
        /* Increment the candidate integer */
        ++last_checked;

        /* Test if last_checked is prime */
        if (!is_prime_big((seq_t)last_checked))
            continue; /* not prime, skip */

        /* Compute 2p +/- {1,3} */
        seq_t p = (seq_t)last_checked;
        seq_t two = (seq_t)2;

        seq_t val1 = two * p - (seq_t)3; /* 2p-3 */
        if (is_prime_big(val1))
            continue; /* composite condition violated */

        seq_t val2 = two * p - (seq_t)1; /* 2p-1 */
        if (is_prime_big(val2))
            continue;

        seq_t val3 = two * p + (seq_t)1; /* 2p+1 */
        if (is_prime_big(val3))
            continue;

        seq_t val4 = two * p + (seq_t)3; /* 2p+3 */
        if (is_prime_big(val4))
            continue;

        /* All four numbers are composite, store p */
        grow_seq(&cache, &cap, sz + 1);
        cache[sz++] = p;
    }

    return cache[n - 1];
}
