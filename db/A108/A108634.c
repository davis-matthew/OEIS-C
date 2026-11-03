/*
 * A108634  Semiprimes with ordered digits.
 * https://oeis.org/A108634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 601, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of semiprimes with nondecreasing decimal digits
   (OEIS A108634) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Check whether decimal digits of x are nondecreasing from left to right. */
static bool digits_ordered(uint64_t x)
{
    int digits[20];
    int len = 0;
    while (x > 0) {
        digits[len++] = x % 10;
        x /= 10;
    }
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] > digits[i - 1]) return false;
    }
    return true;
}

static seq_t *seq_array = NULL;   /* dynamic array of found terms */
static size_t   seq_cap   = 0;
static idx_t    seq_size  = 0;
static uint64_t next_candidate = 4;   /* smallest number to test */

/* Ensure the array can hold at least n elements. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_array, new_cap);
    if (!tmp) abort();
    seq_array = tmp;
    seq_cap = new_cap;
}

/* Generate terms of the sequence until at least n terms are stored. */
static void generate_up_to(idx_t n)
{
    idx_t max_needed = n;
    while (seq_size < max_needed) {
        if (is_semiprime_uint64(next_candidate) && digits_ordered(next_candidate)) {
            ensure_capacity(seq_size + 1);
            seq_array[seq_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) abort();  /* index is 1‑based, but we guard anyway */
    generate_up_to(n);
    return seq_array[n - 1];
}
