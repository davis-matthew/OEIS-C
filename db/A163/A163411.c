/*
 * A163411  A positive integer is included if it is a palindrome when written
 *          in binary, and it is divisible by at least one prime that is not a
 *          binary palindrome.
 * https://oeis.org/A163411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A163411:
   a(n) = the nth positive integer that is a binary palindrome and has
   at least one prime divisor that is not a binary palindrome.
   The function generates binary palindromes in increasing order,
   factors each one using factor_u64, and keeps the ones that satisfy
   the divisibility condition.  The results are cached so that
   arbitrary order of calls is supported. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static bool is_bin_palindrome(uint64_t x)
{
    if (x == 0) return false;          /* not considered */
    int len = 64 - __builtin_clzll(x);  /* number of bits */
    for (int i = 0; i < len / 2; ++i) {
        if (((x >> i) & 1ULL) != ((x >> (len - 1 - i)) & 1ULL))
            return false;
    }
    return true;
}

static bool has_nonpal_prime_factor(uint64_t n)
{
    /* factor_u64 writes factor primes into the array pr and outputs
       the number of distinct prime factors in *cnt.  We allocate
       a sufficient buffer on the stack. */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (!is_bin_palindrome(pr[i]))
            return true;
    }
    return false;
}

static seq_t *seq_terms = NULL;   /* cached terms */
static size_t seq_len   = 0;      /* number of cached terms */
static size_t seq_cap   = 0;      /* current capacity of seq_terms */
static uint64_t last_checked = 0; /* last integer examined */

/* Allocate or grow the array that stores the sequence terms. */
static void ensure_capacity(void)
{
    if (seq_cap == 0) {
        seq_cap = 128;
        seq_terms = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else {
        seq_cap <<= 1;
        seq_terms = (seq_t *)realloc(seq_terms, seq_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    /* Ensure sequence has at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (!is_bin_palindrome(last_checked))
            continue;
        if (has_nonpal_prime_factor(last_checked)) {
            if (seq_len == seq_cap)
                ensure_capacity();
            seq_terms[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq_terms[n - 1];
}
