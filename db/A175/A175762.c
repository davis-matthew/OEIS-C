/*
 * A175762  Primes with an arithmetic mean of digits which is also prime.
 * https://oeis.org/A175762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 599, u32 n <= 602, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements the OEIS formula:
 *   a(n) = the nth prime p such that (sum of decimal digits of p)
 *           divided by (number of decimal digits of p) is an integer
 *           prime.
 *
 * The function enumerates primes in increasing order and tests each
 * candidate until it has found n qualifying primes.  Results are cached
 * in a static array so that calls with arbitrary n order are fast.
 */
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions from the build */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t) */

static seq_t *term_list = NULL;   /* cached results */
static size_t term_len  = 0;      /* number of cached terms */
static size_t term_cap  = 0;      /* allocated capacity */

static seq_t last_checked = 1;    /* last integer examined */

/* Expand the cached term array lazily */
static void grow_term_list(void)
{
    size_t new_cap = (term_cap == 0) ? 8 : term_cap << 1;
    seq_t *tmp = (seq_t *)realloc(term_list, sizeof(seq_t) * new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    term_list = tmp;
    term_cap  = new_cap;
}

/* Append a new term to the cache */
static void add_term(seq_t val)
{
    if (term_len == term_cap) grow_term_list();
    term_list[term_len++] = val;
}

/* Count decimal digits of a number */
static size_t digit_count(seq_t x)
{
    if (x == 0) return 1;
    size_t d = 0;
    seq_t v = x;
    while (v > 0) {
        v /= 10;
        ++d;
    }
    return d;
}

/* Sum decimal digits of a number */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    seq_t v = x;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but safe return */

    /* Initialise static storage on first call */
    if (!term_list) {
        term_cap = 8;
        term_list = (seq_t *)malloc(sizeof(seq_t) * term_cap);
        term_len  = 0;
    }

    /* Generate terms until we have at least n of them */
    while (term_len < n) {
        /* Find next prime candidate after last_checked */
        seq_t cand = last_checked + 1;
        /* For efficiency skip even numbers */
        if (cand > 2 && (cand & 1) == 0) ++cand;

        for (;;) {
            if (is_prime_big(cand)) {
                /* Evaluate the digit mean condition */
                seq_t sum = digit_sum(cand);
                size_t d   = digit_count(cand);
                if ((seq_t)d != 0 && (sum % (seq_t)d) == 0) {
                    seq_t mean = sum / (seq_t)d;
                    if (is_prime_big(mean)) {
                        add_term(cand);
                    }
                }
                last_checked = cand;   /* next search starts after this */
                break;               /* go to next candidate */
            }
            cand += 2;               /* skip evens */
        }
    }

    return term_list[n - 1];         /* index offset 1-based */
}
