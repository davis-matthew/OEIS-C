/*
 * A036976  Becomes a prime if a 1 is inserted before each digit.
 * https://oeis.org/A036976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n such that inserting a '1' before each decimal digit of n produces a prime. */
/* This function generates terms lazily and memoises them across calls. */

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
    /* Static cache of found terms. */
    static seq_t *terms = NULL;
    static idx_t    size   = 0;   /* number of cached terms */
    static idx_t    capacity = 0; /* allocated capacity */
    static idx_t    next_candidate = 1; /* integer to test next */

    /* Expand cache until it contains at least n+1 terms. */
    while (size <= n) {
        /* Build the number obtained by inserting '1' before each digit of next_candidate. */
        seq_t inserted = 0;
        idx_t temp = next_candidate;

        /* Extract digits into a small buffer (least significant first). */
        unsigned char digits[20];  /* 20 digits safely covers 64‑bit integers */
        idx_t len = 0;
        if (temp == 0) {
            digits[len++] = 0;
        } else {
            while (temp) {
                digits[len++] = temp % 10;
                temp /= 10;
            }
        }

        /* Process digits from most significant to least. */
        for (idx_t i = len; i-- > 0; ) {
            unsigned char d = digits[i];
            inserted = inserted * 100 + (10 + d);
        }

        /* Test primality of the inserted value. */
        if (is_prime_big(inserted)) {
            /* Grow cache if needed. */
            if (size == capacity) {
                capacity = capacity ? capacity * 2 : 4;
                terms = mem_resize(seq_t, terms, capacity);
            }
            terms[size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return terms[n];
}
