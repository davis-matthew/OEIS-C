/*
 * A142428  Primes congruent to 17 mod 49.
 * https://oeis.org/A142428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 17 modulo 49
 * Range:     u8 n <= 1, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 993 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: test numbers of the form 17 + 49·k for primality,
   storing found primes in a dynamically grown static array. */

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

seq_t a(idx_t n)
{
    /* Static cache of found primes and state of the search */
    static seq_t *values = NULL;   /* dynamic array of primes */
    static size_t size  = 0;        /* number of primes stored   */
    static size_t cap   = 0;        /* current capacity of array */
    static idx_t  next_k = 0;       /* next k such that candidate = 17 + 49·k */

    if (n == 0)                      /* sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Initialise the cache lazily. */
    if (values == NULL) {
        cap   = 128;
        values = mem_new(seq_t, cap);
        if (!values) exit(1);        /* abort on allocation failure */
    }

    /* Generate primes until we have at least n of them. */
    while (size < n) {
        idx_t candidate_idx = 17 + 49 * next_k;
        ++next_k;

        seq_t candidate = (seq_t)candidate_idx;
        if (is_prime_big(candidate)) {
            /* Ensure enough space in the array. */
            if (size == cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, values, newcap);
                if (!tmp) exit(1);    /* abort on allocation failure */
                values = tmp;
                cap = newcap;
            }
            values[size++] = candidate;
        }
    }

    return values[n - 1];
}
