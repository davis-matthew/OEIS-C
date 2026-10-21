/*
 * A054546  First differences of nonprimes (including 0 and 1, A002808).
 * https://oeis.org/A054546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth nonprime minus previous nonprime (first differences
 *            of the nonprimes, including 0 and 1) a(n) = A018252(n) -
 *            A141468(n), offset 1 Compute by generating nonprimes and storing
 *            their differences
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* The build defines seq_t and idx_t. */

/* Static data for memoisation */
static seq_t *a_seq = NULL;          /* array of a(n) */
static size_t a_cap = 0;             /* allocated capacity */
static size_t a_len = 0;             /* number of computed terms */

static bool *is_prime = NULL;        /* sieve array */
static size_t sieve_limit = 0;       /* current sieve upper bound */

static uint64_t current_candidate = 1;   /* next integer to test */
static uint64_t prev_nonprime = 0;       /* previous nonprime (starting with 0) */

/* Extend the prime sieve to cover at least 'limit' (inclusive) */
static void extend_sieve(size_t limit) {
    if (limit <= sieve_limit)
        return;
    size_t new_limit = limit + 1;
    bool *new_is_prime = (bool *)malloc(new_limit);
    if (!new_is_prime)
        abort();            /* OOM */
    for (size_t i = 0; i < new_limit; ++i)
        new_is_prime[i] = true;
    if (new_limit > 0) {
        new_is_prime[0] = false;
        if (new_limit > 1)
            new_is_prime[1] = false;
    }
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_is_prime[j] = false;
        }
    }
    free(is_prime);
    is_prime = new_is_prime;
    sieve_limit = limit;
}

/* Ensure that a(n) is computed for all n up to 'need' */
static void ensure_a(idx_t need) {
    if ((size_t)need <= a_len)
        return;

    /* Grow storage for a_seq */
    if (a_cap == 0) {
        a_cap = 16;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
    }
    while (a_cap < (size_t)need) {
        a_cap *= 2;
        a_seq = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
        if (!a_seq)
            abort();
    }

    /* Generate a_seq[ a_len ... need-1 ] */
    for (size_t target = a_len; target < (size_t)need; ++target) {
        /* Find next nonprime */
        while (true) {
            /* Ensure sieve covers current_candidate */
            if (current_candidate > sieve_limit)
                extend_sieve(current_candidate * 2);
            if (!is_prime[(size_t)current_candidate]) {
                seq_t diff = (seq_t)(current_candidate - prev_nonprime);
                a_seq[a_len++] = diff;
                prev_nonprime = current_candidate;
                ++current_candidate;
                break;
            }
            ++current_candidate;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n<1, but return 0 safely */
    ensure_a(n);
    return a_seq[n - 1];
}
