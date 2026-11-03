/*
 * A032670  Digit '7' concatenated with a(n) is a prime.
 * https://oeis.org/A032670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 482, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9386 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer m such that the concatenation of digit 7
 * and m (i.e., 7…m in decimal) is a prime number. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

static seq_t *seq = NULL;            /* array of found terms, 0‑based storage */
static size_t seq_cap = 0;           /* current allocated capacity */
static idx_t  seq_len = 0;           /* number of terms stored already */
static seq_t  next_candidate = (seq_t)1;  /* next integer to test */

/* Grow the internal cache if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < seq_len; ++i)
        new_arr[i] = seq[i];
    if (seq)
        seq_free(seq);
    seq = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, return 0 */

    /* Acquire results up to n */
    while (seq_len < n) {
        /* Find next candidate whose 7‑prefixed number is prime */
        for (;;) {
            /* Build 7…cand */
            seq_t power = pow10(num_digits(next_candidate));
            seq_t concat = (seq_t)7 * power + next_candidate;
            if (isPrimeGeneric(concat)) {
                /* Store the candidate */
                ensure_capacity(seq_len + 1);
                seq[seq_len] = next_candidate;
                ++seq_len;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return seq[n - 1];
}
