/*
 * A091242  Reducible polynomials over GF(2), coded in binary.
 * https://oeis.org/A091242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091242: reducible polynomials over GF(2), coded in binary.
   Computation: iterate natural numbers, test each for reducibility over GF(2),
   store the n‑th reducible polynomial. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* Multiply polynomials over GF(2) – XOR‑multiplication. */
static seq_t poly_mul(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (b) {
        if (b & 1)
            res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

/* Remainder of dividend divided by divisor over GF(2). */
static seq_t poly_mod(seq_t dividend, seq_t divisor)
{
    if (divisor == 0)
        return dividend;

    size_t div_deg = bitlen_seq_t(divisor) - 1;
    seq_t rem = dividend;

    while (rem != 0 && bitlen_seq_t(rem) > div_deg) {
        size_t rem_deg = bitlen_seq_t(rem) - 1;
        size_t shift   = rem_deg - div_deg;
        rem ^= (divisor << shift);
    }
    return rem;
}

/* Return true if integer p encodes a reducible polynomial over GF(2). */
static bool is_reducible(seq_t p)
{
    /* Constants and linear polynomials are not reducible. */
    if (p < 4)
        return false;

    size_t deg = bitlen_seq_t(p) - 1;
    if (deg <= 1)
        return false;

    /* Easy test: divisibility by a power of x (trailing zero bits). */
    for (size_t d = 1; d <= deg / 2; ++d) {
        seq_t mask = ((seq_t)1 << d) - 1;
        if ((p & mask) == 0)
            return true;
    }

    /* Exhaustive search of all possible factors of degree <= deg/2. */
    for (size_t d = 1; d <= deg / 2; ++d) {
        seq_t start = ((seq_t)1 << d);
        seq_t end   = ((seq_t)1 << (d + 1)) - 1;
        for (seq_t q = start; q <= end; ++q) {
            if (poly_mod(p, q) == 0)
                return true;
        }
    }
    return false;
}

/* Dynamic cache of the sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;          /* number of terms stored exactly. */
static size_t cache_cap  = 0;          /* allocated capacity.   */
static idx_t last_checked = 0;         /* last integer examined. */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is 1‑based; undefined for n==0 */
        return (seq_t)0;

    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t cur = (seq_t)last_checked;
        if (is_reducible(cur)) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = cur;
        }
    }
    return cache[n - 1];
}
