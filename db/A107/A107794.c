/*
 * A107794  Numbers n such that both n plus its first digit and n minus its
 *          first digit are primes.
 * https://oeis.org/A107794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107794: numbers n such that n+first_digit(n) and n-first_digit(n) are prime */
/* The function returns the nth term (1‑based) of this sequence. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Return the most significant decimal digit of a 64‑bit integer */
static unsigned long long leading_digit_of_uint64(unsigned long long v)
{
    while (v >= 10) v /= 10;
    return v;
}

static seq_t *terms = NULL;
static idx_t terms_cap = 0;
static idx_t terms_cnt = 0;
static uint64_t cur = 12;   /* next candidate to test for property */

static void add_term(seq_t val)
{
    if (terms_cnt == terms_cap) {
        terms_cap = terms_cap ? 2 * terms_cap : 64;
        terms = mem_resize(seq_t, terms, terms_cap);
    }
    terms[terms_cnt++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, ignore 0 */

    while (terms_cnt < n) {
        uint64_t f = leading_digit_of_uint64(cur);
        if (cur > f) { /* ensure n-f >= 1 */
            if (is_prime_u64(cur + f) && is_prime_u64(cur - f)) {
                add_term((seq_t)cur);
            }
        }
        cur++;
    }
    return terms[n - 1];
}
