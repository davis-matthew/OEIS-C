/*
 * A167466  Primes p such that (p reversed)+8 is also a prime.
 * https://oeis.org/A167466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that reverse_decimal(p)+8 is also
 *            prime
 * Range:     u8 n <= 7, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 951 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
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

static seq_t *terms = NULL;          /* array of found terms   */
static size_t terms_size = 0;        /* number of stored terms */
static size_t terms_cap  = 0;        /* current allocation size */
static idx_t   next_candidate = 2;   /* next integer to test   */

/* Grow the terms array when more space is required. */
static void grow_terms(void) {
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    if (!tmp) abort();               /* out of memory            */
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the nth term of the sequence (n ≥ 1).  Calls may be out of order. */
seq_t a(idx_t n) {
    /* Ensure that at least n terms have been computed. */
    while (terms_size < (size_t)n) {
        /* Skip even numbers > 2, they cannot be prime. */
        if (next_candidate > 2 && ((next_candidate & 1ULL) == 0)) {
            next_candidate++;
            continue;
        }

        seq_t p = (seq_t)next_candidate;
        if (is_prime_big(p)) {
            seq_t revp = reverse_digits(p);
            seq_t revplus8 = revp + (seq_t)8;   /* same type as revp   */
            if (is_prime_big(revplus8)) {
                if (terms_size == terms_cap) grow_terms();
                terms[terms_size++] = p;
            }
        }
        next_candidate++;
    }
    return terms[(size_t)n - 1];
}
