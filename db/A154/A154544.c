/*
 * A154544  Primes p such that the smallest digit of p + the number of
 *          smallest digits of p is an even composite number.
 * https://oeis.org/A154544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 686, u32 n <= 957, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the smallest digit of p plus the
 *   number of its occurrences is an even composite number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* tests primality for arbitrary seq_t */
#include "../utils/is_prime_small.h"/* tests small integers (e.g. composite check) */

static uint64_t next_candidate = 1;   /* small integer to test for primality */
static size_t seq_cap = 0;             /* capacity of cached array */
static size_t seq_len = 0;             /* number of cached terms */
static seq_t *seq_vals = NULL;         /* cache of sequence values */

/* Ensure the cache has at least 'needed' slots. */
static void grow_seq_array(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = (seq_cap > 0) ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();
    seq_vals = new_arr;
    seq_cap = new_cap;
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Generate terms until we have the nth one. */
    while (seq_len < (size_t)n) {
        /* Find next prime candidate. */
        do {
            ++next_candidate;
        } while (!is_prime_big((seq_t)next_candidate));

        seq_t p = (seq_t)next_candidate;

        /* Determine the smallest digit and its count. */
        seq_t tmp = p;
        seq_t min_digit = 10;   /* greater than any decimal digit */
        while (tmp > 0) {
            seq_t d = tmp % 10;
            if (d < min_digit)
                min_digit = d;
            tmp /= 10;
        }

        tmp = p;
        seq_t count = 0;
        while (tmp > 0) {
            seq_t d = tmp % 10;
            if (d == min_digit)
                ++count;
            tmp /= 10;
        }

        seq_t sum = min_digit + count;
        /* Check if sum is even composite. */
        if ((sum % 2 == 0) && (sum > 2) && !is_prime_small(sum)) {
            grow_seq_array(seq_len + 1);
            seq_vals[seq_len++] = p;
        }
    }

    return seq_vals[(size_t)n - 1];
}
