/*
 * A030462  Palindromic prime concatenated with next palindromic prime is a
 *          prime.
 * https://oeis.org/A030462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030462: a(n) is the nth palindromic prime p_k such that concat(p_k,p_{k+1}) is prime.
   The function generates palindromic primes in order, tests the next pair
   for primality, and caches those that satisfy the condition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
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

/* Build a palindrome from a half part.  If odd == true, the half omits the
   middle digit when mirroring. */
static seq_t palindrome_from_half(seq_t half, bool odd)
{
    seq_t pal = half;
    seq_t x = odd ? half / 10 : half;
    while (x != 0) {
        pal = pal * 10 + (x % 10);
        x /= 10;
    }
    return pal;
}

/* Generator that returns the next palindromic prime in ascending order. */
static seq_t get_next_pal_prime(void)
{
    static idx_t len = 1;               /* current palindrome length */
    static idx_t half_len = 1;          /* length of the half part   */
    static idx_t half_start = 1;        /* first value of current half */
    static idx_t half_end   = 9;        /* last value of current half  */
    static idx_t half_cur   = 1;        /* current half value         */
    static bool odd_flag = true;        /* true if length is odd      */

    while (true) {
        if (half_cur > half_end) {      /* move to next length      */
            ++len;
            half_len = (len + 1) / 2;
            half_start = 1;
            for (idx_t i = 1; i < half_len; ++i) half_start *= 10;
            half_end = half_start * 10 - 1;
            half_cur = half_start;
            odd_flag = (len % 2 == 1);
        }

        seq_t cand = palindrome_from_half((seq_t)half_cur, odd_flag);
        ++half_cur;

        if (cand >= 2 && isPrimeGeneric(cand))
            return cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0 */

    /* static state for memoisation */
    static seq_t *special = NULL;   /* cache of qualifying palindromic primes */
    static size_t spec_size = 0;    /* number of stored terms          */
    static size_t spec_cap  = 0;    /* current capacity                 */
    static seq_t prev_pal = 0;      /* previous palindrome prime       */
    static bool first_flag = true;  /* true before first prime processed */

    /* generate until we have at least n terms */
    while (spec_size < n) {
        seq_t curr = get_next_pal_prime();

        if (first_flag) {
            prev_pal   = curr;
            first_flag = false;
            continue;
        }

        seq_t concat = concat_parts((idx_t)prev_pal, (idx_t)curr);
        if (isPrimeGeneric(concat)) {    /* qualifies as a(n) */
            if (spec_size == spec_cap) {
                size_t new_cap = (spec_cap == 0 ? 4 : spec_cap << 1);
                special = seq_realloc(special, spec_cap, new_cap);
                spec_cap = new_cap;
            }
            special[spec_size++] = prev_pal;
        }

        prev_pal = curr;
    }

    return special[n - 1];   /* zero‑based array indexing */
}
