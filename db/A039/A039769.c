/*
 * A039769  Composite integers k such that gcd(phi(k), k - 1) > 1.
 * https://oeis.org/A039769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6646, big any n
 * Helpers:   gcd, is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039769: composite integers k such that gcd(phi(k), k-1) > 1 */
/* The function builds the sequence by scanning integers and collecting
   those that satisfy the condition.  It memoises found terms so that
   any call to a(idx_t n) returns successfully even when requests
   are out of order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static data for memoisation */
    static seq_t *seq = NULL;      /* array of found terms                */
    static size_t seq_cap = 0;     /* current capacity of array           */
    static size_t seq_len = 0;     /* number of terms stored so far       */
    static idx_t next = 4;         /* next candidate integer to examine   */

    /* If we already have the nth term, just return it. */
    if (n <= seq_len)
        return seq[n - 1];

    /* Grow array capacity as needed */
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* abort on OOM */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Scan numbers from 'next' until we have collected enough terms */
    while (seq_len < n) {
        /* Skip primes and numbers <=3 */
        while (next <= 3 || is_prime(next))
            ++next;

        /* Compute phi(next) and gcd(phi, next-1) */
        seq_t phi_val = phi(next);
        seq_t g = gcd(phi_val, (seq_t)(next - 1));

        if (g > 1) {
            /* Store the term */
            seq[seq_len++] = (seq_t)next;
        }
        ++next;
    }

    return seq[n - 1];
}
