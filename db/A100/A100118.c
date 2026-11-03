/*
 * A100118  Numbers whose sum of prime factors is prime (counted with
 *          multiplicity).
 * https://oeis.org/A100118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9920 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a simple sieve: return the nth integer n (n≥1) whose
   sum of prime factors (with multiplicity), sopfr(n), is itself prime.
   The sequence starts 2,3,5,6,7,10,11,... as in OEIS A100118. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static uint64_t sopfr(uint64_t n) {
    uint64_t sum = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        while (tmp % p == 0) {
            sum += p;
            tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp;  /* remaining prime factor */
    return sum;
}

static bool is_prime_sum(uint64_t s) {
    /* Uses the provided is_prime_uint64 helper for sums up to 64‑bit. */
    return is_prime_u64(s);
}

static seq_t* buffer = NULL;       /* array of found terms */
static idx_t   buf_cap = 0;        /* capacity of buffer */
static idx_t   buf_len = 0;        /* number of terms stored */
static uint64_t next_candidate = 2;/* next integer to test */

static void grow_buffer(void) {
    idx_t new_cap = (buf_cap == 0) ? 128 : buf_cap * 2;
    seq_t* tmp = mem_resize(seq_t, buffer, new_cap);
    buffer = tmp;
    buf_cap = new_cap;
}

static void collect_terms_to(idx_t target) {
    while (buf_len < target) {
        uint64_t val = next_candidate;
        if (is_prime_sum(sopfr(val))) {
            if (buf_len == buf_cap) grow_buffer();
            buffer[buf_len++] = (seq_t)val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset not defined; return 0 */
    if (buf_len < n) collect_terms_to(n);
    return buffer[n - 1];
}
