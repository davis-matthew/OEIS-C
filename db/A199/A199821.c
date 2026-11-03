/*
 * A199821  Primes of form 12^n-11^n.
 * https://oeis.org/A199821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime of the form 12^k – 11^k for k ≥ 2. */

#include <stddef.h>
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
    /* Static state for out‑of‑order calls */
    static seq_t *primes      = NULL;
    static size_t primes_cnt  = 0;
    static size_t primes_cap  = 0;
    static idx_t  curr_k      = 2;          /* exponent currently examined   */
    static seq_t  pow12        = 0;          /* 12^curr_k                      */
    static seq_t  pow11        = 0;          /* 11^curr_k                      */
    static bool   initialized = false;

    /* Initialise the first powers for k = 2 (12² and 11²). */
    if (!initialized) {
        pow12 = (seq_t)12 * (seq_t)12;       /* 12²  = 144                     */
        pow11 = (seq_t)11 * (seq_t)11;       /* 11²  = 121                     */
        initialized = true;
    }

    /* Generate primes until at least n of them are found. */
    while (primes_cnt < n) {
        seq_t candidate = pow12 - pow11;     /* 12^k – 11^k                    */

        if (is_prime_big(candidate)) {
            /* Grow the output array if necessary. */
            if (primes_cnt == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 8 : primes_cap * 2;
                primes = seq_realloc(primes, primes_cap, new_cap);
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }

        /* Advance to the next exponent. */
        ++curr_k;
        pow12 *= (seq_t)12;
        pow11 *= (seq_t)11;
    }

    return primes[n - 1];
}
