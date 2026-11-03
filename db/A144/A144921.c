/*
 * A144921  Duplicate of A002476.
 * https://oeis.org/A144921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 1 (mod 6) – i.e. primes of the form 6*k+1 */

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
    /* a(1) = 7, a(2) = 13, …  */
    if (n == 0)
        return 0;   /* value below the offset is undefined */

    /* Cache for the primes of the form 6k+1 found so far. */
    static seq_t *list = NULL;
    static size_t len   = 0;     /* number of primes stored */
    static size_t cap   = 0;     /* allocated capacity */

    /* Next candidate of the form 6k+1.  Start at 7. */
    static seq_t next = (seq_t)7;

    /* Initialise the cache on the first call. */
    if (len == 0) {
        cap   = 16;
        list  = mem_new(seq_t, cap);
    }

    /* Generate new primes until we have at least n of them. */
    while (len < (size_t)n) {
        /* If the candidate is prime, store it. */
        if (is_prime_big(next)) {
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                list = mem_resize(seq_t, list, cap);
            }
            list[len++] = next;
        }
        /* Next candidate of the form 6k+1. */
        next += 6;
    }

    return list[(size_t)n - 1];
}
