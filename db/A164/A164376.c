/*
 * A164376  Nonprime numbers that are not a sum of 2 primes.
 * https://oeis.org/A164376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0, 1, followed by all odd composite numbers n such that
 *            n-2 is not prime (offset 1)
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build the sequence up to a numeric limit `lim`.
   *seq_ptr receives the new array; *len_ptr receives its length (including the first two 0,1). */
static void build_sequence(uint64_t lim, seq_t **seq_ptr, size_t *len_ptr)
{
    /* Sieve of Eratosthenes up to lim */
    bool *is_prime = (bool *)malloc((lim + 1u) * sizeof(bool));
    for (uint64_t i = 0; i <= lim; ++i) is_prime[i] = true;
    is_prime[0] = false;
    if (lim >= 1) is_prime[1] = false;

    for (uint64_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= lim; j += i)
                is_prime[j] = false;
        }
    }

    /* Gather qualifying odd composite numbers */
    size_t temp_cap = (lim >> 1) + 10;          /* rough estimate of odd composites */
    seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
    size_t tlen = 0;

    for (uint64_t num = 3; num <= lim; num += 2) {
        if (!is_prime[num]) {                              /* composite */
            if (num < 2 || !is_prime[num - 2]) {           /* n‑2 is composite (or <2) */
                if (tlen == temp_cap) {                    /* grow temporary array */
                    temp_cap <<= 1;
                    temp = (seq_t *)realloc(temp, temp_cap * sizeof(seq_t));
                }
                temp[tlen++] = (seq_t)num;
            }
        }
    }
    free(is_prime);

    /* Allocate final sequence with 0,1 prefix */
    size_t new_cap = 2 + tlen;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    new_seq[0] = (seq_t)0;
    new_seq[1] = (seq_t)1;
    for (size_t i = 0; i < tlen; ++i)
        new_seq[2 + i] = temp[i];
    free(temp);

    *seq_ptr = new_seq;
    *len_ptr = new_cap;
}

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *seq        = NULL;
    static size_t seq_len    = 0;
    static uint64_t limit    = 10;     /* numeric limit for the sieve */

    /* Initialise on first call */
    if (!seq) {
        build_sequence(limit, &seq, &seq_len);
    }

    /* Ensure at least n terms are available */
    while ((idx_t)seq_len < n) {
        limit *= 2;                    /* increase numeric limit */
        if (limit < 10) limit = 10;

        seq_t *old_seq = seq;
        build_sequence(limit, &seq, &seq_len);
        free(old_seq);
    }

    /* Return the requested element (offset 1) */
    return seq[(size_t)(n - 1)];
}
