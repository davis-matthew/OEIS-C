/*
 * A104390  2-Smith numbers.
 * https://oeis.org/A104390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 813, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9762 (2026-11-03)
 * Helpers:   digit_sum_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑Smith numbers: a(n) is the n‑th integer >1 whose sum of digit sums of its
 * prime factors (counted with multiplicity) equals twice the digit sum of the
 * integer itself.  The function determines the terms by brute‑force search
 * using trial‑division factorisation.  No analytic formula is known.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/mem.h"

/*  Computes the digit sum of prime factors (with multiplicity) of n  */
static uint64_t sp_of(uint64_t n)
{
    uint64_t sp = 0;
    /* factor 2 separately */
    if ((n & 1) == 0) {
        uint64_t exp = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++exp;
        }
        sp += exp * digit_sum_u64(2ULL);
    }
    /* odd factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            sp += exp * digit_sum_u64(p);
        }
    }
    /* remainder is a prime > sqrt(original n) */
    if (n > 1) {
        sp += digit_sum_u64(n);
    }
    return sp;
}

/*--------- test whether a 64‑bit integer is a 2‑Smith number ---------*/
static bool is_two_smith(uint64_t x)
{
    if (x < 32) return false;          /* the first 2‑Smith number is 32 */
    return sp_of(x) == 2 * digit_sum_u64(x);
}

/*--------- helper: trial‑division factorisation (64‑bit) -------------*/

/*--------- public function: the sequence a(n) -----------------------*/
seq_t a(idx_t n)
{
    /* Lazy static cache of found 2‑Smith numbers */
    static seq_t *terms    = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    /* candidate counter; starts at the first possible 2‑Smith number */
    static uint64_t next_candidate = 32;

    /* grow cache if needed */
    if (terms == NULL) {
        terms_cap = 64;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) exit(EXIT_FAILURE);
    }
    while (terms_len < (size_t)n) {
        /* advance to next candidate having the property */
        while (!is_two_smith(next_candidate)) ++next_candidate;
        /* store the found term */
        terms[terms_len] = (seq_t)next_candidate;
        ++terms_len;
        ++next_candidate;
        /* enlarge cache when full */
        if (terms_len == terms_cap) {
            terms_cap <<= 1;
            size_t new_bytes = terms_cap * sizeof(seq_t);
            seq_t *tmp = mem_resize(seq_t, terms, (new_bytes) / sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            terms = tmp;
        }
    }

    return terms[n - 1];
}
