/*
 * A154716  Products of three consecutive happy primes A035497.
 * https://oeis.org/A154716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A154716: products of three consecutive happy primes
 * Formula: a(n) = p_n * p_{n+1} * p_{n+2}
 * where p_k is the k-th happy prime (first happy prime is 7).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t *happy_primes = NULL;
static size_t happy_count = 0;
static size_t happy_cap = 0;
static uint64_t last_checked = 0;

/* Test if a 64‑bit number is happy */
static bool is_happy_uint64(uint64_t n) {
    bool seen[2000] = {0};
    uint64_t x = n;
    while (1) {
        uint64_t sum = 0;
        uint64_t t = x;
        while (t > 0) {
            uint64_t d = t % 10;
            sum += d * d;
            t /= 10;
        }
        if (sum == 1) return true;
        if (seen[sum]) return false;
        seen[sum] = true;
        x = sum;
    }
}

/* Ensure capacity for happy_primes array */
static void ensure_happy_capacity(size_t needed) {
    if (happy_cap >= needed) return;
    size_t new_cap = happy_cap ? happy_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *new_arr = (uint64_t *)realloc(happy_primes, new_cap * sizeof(uint64_t));
    if (!new_arr) exit(EXIT_FAILURE);
    happy_primes = new_arr;
    happy_cap = new_cap;
}

/* Populate happy primes until we have at least required_count entries */
static void generate_happy_primes(idx_t required_count) {
    while (happy_count < (size_t)required_count) {
        uint64_t p = last_checked + 1;
        for (;;) {
            ++p;
            if (p < 2) continue;
            /* Only test odd candidates (2 handled separately) */
            if (p == 2 || (p & 1)) {
                if (is_prime((idx_t)p)) {
                    if (is_happy_uint64(p)) {
                        ensure_happy_capacity(happy_count + 1);
                        happy_primes[happy_count++] = p;
                    }
                    break; /* prime found, advance last_checked */
                }
            }
        }
        last_checked = p;
    }
}

seq_t a(idx_t n) {
    /* a(1) requires indices 0,1,2 => need at least 3 happy primes */
    if (n < 1) return 0; /* behavior for n<1 is irrelevant */
    size_t required = (size_t)n + 2; /* indices up to n+1 inclusive */
    generate_happy_primes(required);
    seq_t x = (seq_t)happy_primes[(size_t)n - 1];
    seq_t y = (seq_t)happy_primes[(size_t)n];
    seq_t z = (seq_t)happy_primes[(size_t)n + 1];
    return x * y * z;
}
