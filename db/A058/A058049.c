/*
 * A058049  Numbers k such that the sum of the digits of the first k primes is
 *          a prime.
 * https://oeis.org/A058049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digit_sum_u64, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058049: Numbers k such that the sum of the digits of the first k primes is a prime.
   Implementation: sequentially generate primes, keep a running sum of their digit sums,
   and record the prime count whenever that sum is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1) is the first term */
        return 0;

    /* ---------- static state persist between calls ---------- */
    static seq_t *terms = NULL;        /* array of sequence terms (k values) */
    static size_t terms_cap = 0;        /* current capacity of terms array */
    static size_t terms_len = 0;        /* number of terms already computed */

    static seq_t digit_sum = 0;         /* cumulative sum of digits of processed primes */
    static idx_t prime_cnt = 0;         /* count of primes processed so far */
    static uint64_t cur_prime = 1;      /* last prime found (next_prime(1) -> 2) */

    size_t target = (size_t)n;         /* needed number of terms */

    /* ---- grow terms array if necessary ---- */
    if (terms_cap < target) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }

    /* ---- generate terms until we have at least `target` terms ---- */
    while (terms_len < target) {
        cur_prime = next_prime(cur_prime);
        prime_cnt++;

        /* add digit sum of the new prime to the running total */
        digit_sum += (seq_t)digit_sum_u64(cur_prime);

        /* if the cumulative digit sum is prime, record the current prime count */
        if (is_prime_big(digit_sum)) {
            if (terms_len == terms_cap) {   /* grow array if we hit capacity */
                size_t new_cap = terms_cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)prime_cnt;
        }
    }

    return terms[n - 1];
}
