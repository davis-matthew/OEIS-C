/*
 * A180156  Sequence from adding the twin primes and dividing by the digit sum
 *          of twin primes.
 * https://oeis.org/A180156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = round( (p + q) / digit_sum(p) + digit_sum(q) ) where p
 *            and q are the lower and upper primes of the n‑th twin prime
 *            pair, rounded to the nearest integer (floor(1/2 + x))
 * Range:     u8 n <= 10, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on the supplied primality test
   bool is_prime(idx_t x) and the digit sum routine
   seq_t digit_sum(seq_t v). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"

/* Dynamically growable storage for twin primes.
   twin_p1[i] = lower prime of the (i+1)-st twin pair
   twin_p2[i] = upper prime of the (i+1)-st twin pair   */
static uint64_t *twin_p1   = NULL;
static uint64_t *twin_p2   = NULL;
static size_t   twin_cnt   = 0;
static size_t   twin_cap   = 0;

/* Next odd integer to test for being the lower of a twin pair.
   Start with 3, the first lower twin prime. */
static uint64_t last_candidate = 3;

/* Append a twin pair to the arrays, growing them if necessary. */
static void push_twin(uint64_t p, uint64_t q)
{
    if (twin_cnt == twin_cap) {
        size_t new_cap = twin_cap ? twin_cap * 2 : 16;
        uint64_t *tmp1 = (uint64_t *)realloc(twin_p1, new_cap * sizeof(uint64_t));
        uint64_t *tmp2 = (uint64_t *)realloc(twin_p2, new_cap * sizeof(uint64_t));
        if (!tmp1 || !tmp2) abort();          /* allocation failed */
        twin_p1 = tmp1;
        twin_p2 = tmp2;
        twin_cap = new_cap;
    }
    twin_p1[twin_cnt] = p;
    twin_p2[twin_cnt] = q;
    ++twin_cnt;
}

/* Ensure that at least n twin prime pairs are stored. */
static void ensure_twin_count(idx_t n)
{
    while (twin_cnt < n) {
        uint64_t p = last_candidate;
        if (is_prime(p)) {
            uint64_t q = p + 2;
            if (is_prime(q))
                push_twin(p, q);
        }
        last_candidate += 2;   /* test next odd number */
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* unspecified; harmless */
        return 0;
    ensure_twin_count(n);

    uint64_t p = twin_p1[n - 1];
    uint64_t q = twin_p2[n - 1];

    seq_t sum = (seq_t)p + (seq_t)q;
    seq_t ds  = digit_sum((seq_t)p) + digit_sum((seq_t)q);   /* ds > 0 */

    /* round(x) = floor(1/2 + x) for positive x.  Using integer arithmetic: */
    seq_t result = (sum + ds / 2) / ds;

    return result;
}
