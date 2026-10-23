/*
 * A108257  Numbers k such that concatenating k and the sum of factorials of
 *          the digits of k produces a prime.
 * https://oeis.org/A108257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 769, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime, isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A108257:
   numbers k such that concatenating k and the sum of factorials of
   the decimal digits of k produces a prime.  The function
   iterates over k and caches results so that any a(n) can be
   retrieved in O(1) after an initial O(n) search. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime.h"
#include "../utils/isprime_big.h"

static const seq_t factor[10] = {
    (seq_t)1,            /* 0! */
    (seq_t)1,            /* 1! */
    (seq_t)2,            /* 2! */
    (seq_t)6,            /* 3! */
    (seq_t)24,           /* 4! */
    (seq_t)120,          /* 5! */
    (seq_t)720,          /* 6! */
    (seq_t)5040,         /* 7! */
    (seq_t)40320,        /* 8! */
    (seq_t)362880        /* 9! */
};

/* helper: prime test that works for both 64‑bit and larger values */
static inline bool is_prime_seq(seq_t x)
{
    return (sizeof(seq_t)<=sizeof(uint64_t))
           ? (bool)is_prime((uint64_t)x)
           : isprime_big(x);
}

/* cache of discovered terms */
static seq_t *terms = NULL;   /* array of k values */
static size_t terms_cap = 0;  /* allocated capacity */
static size_t terms_len = 0;  /* current number of stored terms */
static idx_t last_k = 0;      /* last integer examined */

/* allocate or grow the terms array if needed */
static void ensure_terms_cap(size_t min_cap)
{
    if (terms_cap < min_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 16;
        if (new_cap < min_cap)
            new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* sum of factorials of decimal digits of k */
static seq_t digit_factorial_sum(idx_t k)
{
    seq_t sum = 0;
    if (k == 0)
        return factor[0];          /* not used, but safe */
    while (k != 0) {
        int d = (int)(k % 10);
        sum += factor[d];
        k /= 10;
    }
    return sum;
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* extend cache until we have n terms */
    while (terms_len < n) {
        ++last_k;
        seq_t sf = digit_factorial_sum(last_k);
        size_t len = digit_len(sf);                           /* number of digits of sf */
        seq_t concat = ((seq_t)last_k) * pow10(len) + sf;     /* concatenate */
        if (is_prime_seq(concat)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = last_k;
        }
    }
    return terms[n - 1];
}
