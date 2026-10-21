/*
 * A067774  Primes p such that p+2 is not a prime.
 * https://oeis.org/A067774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p+2 is composite (≠ prime)
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* static cache for already generated terms */
static seq_t *seq_cache = NULL;    /* array of found terms    */
static size_t seq_len   = 0;        /* current number of terms */
static size_t seq_cap   = 0;        /* allocated capacity      */
static uint64_t last_checked = 1;   /* number that has been
                                      * examined up to now       */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_seq(idx_t n)
{
    while (seq_len < n) {
        ++last_checked;                       /* next integer to test  */
        if (is_prime((idx_t)last_checked)) {   /* is it prime?          */
            if (!is_prime((idx_t)(last_checked + 2))) {
                /* store it */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 8;
                    seq_cache = (seq_t *)realloc(seq_cache,
                                                 seq_cap * sizeof(seq_t));
                }
                seq_cache[seq_len++] = (seq_t)last_checked;
            }
        }
    }
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1   */
    ensure_seq(n);
    return seq_cache[n - 1];
}
