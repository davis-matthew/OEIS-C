/*
 * A092623  Primes with exactly three prime digits.
 * https://oeis.org/A092623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime that has exactly three prime digits
 *            (2,3,5,7)
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence on demand, memoising the values.
   It relies on the helper function is_prime_big from the build utilities. */

#include <stdbool.h>
#include <stdlib.h>
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

/* Append a value to a dynamically growing array. */
static void append_value(seq_t **array, size_t *len, size_t *cap, seq_t value)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *array, new_cap);
        if (!tmp) abort();            /* allocation failed */
        *array = tmp;
        *cap = new_cap;
    }
    (*array)[*len] = value;
    (*len)++;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1: n == 0 should never be requested, but return 0 for safety. */
    if (n == 0) return 0;

    /* Static memoisation state. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;  /* number of cached terms (>=0) */
    static size_t cache_cap  = 0;  /* allocated capacity */
    static seq_t next_candidate = 223;  /* first odd prime with 3 prime digits */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime with exactly three prime digits. */
        while (1) {
            /* Quick digit filter: count prime digits. */
            seq_t tmp = next_candidate;
            unsigned prime_digit_cnt = 0;
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 10);
                if (d == 2 || d == 3 || d == 5 || d == 7) prime_digit_cnt++;
                tmp /= 10;
            }
            if (prime_digit_cnt == 3 && is_prime_big(next_candidate))
                break; /* found the next required term */

            /* Advance to the next odd candidate. */
            next_candidate += 2;
        }

        /* Store the found term. */
        append_value(&cache, &cache_len, &cache_cap, next_candidate);

        /* Advance past the found prime for the next search. */
        next_candidate += 2;
    }

    return cache[(size_t)n - 1];
}
