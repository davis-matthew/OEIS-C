/*
 * A171504  a(n) = n-th nonprime + n.
 * https://oeis.org/A171504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth nonprime + n
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Compute the sequence A171504: a(n) = (n-th nonprime) + n
   Nonprimes include 1, 4, 6, 8, 9, 10, …   */
seq_t a(idx_t n)
{
    /* Static cache of known nonprimes */
    static seq_t *nonprimes = NULL;   /* array holding the nonprimes found so far */
    static size_t nc_len   = 0;       /* number of elements currently stored */
    static size_t nc_cap   = 0;       /* allocated capacity of the array */

    /* Ensure the cache contains at least n entries.  If it doesn't,
       extend the sequence by testing successive integers until the
       required count is reached. */
    if (nc_len < n) {
        /* Grow the array if necessary before adding new entries. */
        if (nc_cap == 0) {
            /* Initial capacity of 8 should cover early growth. */
            nc_cap = 8;
            nonprimes = seq_realloc(NULL, 0, nc_cap);
        } else if (nc_len == nc_cap) {
            nc_cap <<= 1; /* double the capacity */
            nonprimes = seq_realloc(nonprimes, nc_cap >> 1, nc_cap);
        }

        /* Start from value 1 if we have no entries yet, otherwise
           from the next integer after the last stored nonprime. */
        seq_t cur = (nc_len == 0) ? 1 : nonprimes[nc_len - 1] + 1;

        while (nc_len < n) {
            /* Add to cache if cur is not a prime (i.e., is a nonprime). */
            if (!is_prime_big(cur)) {
                nonprimes[nc_len] = cur;
                ++nc_len;
            }
            /* Next candidate.  The addition is safe for arbitrary‑precision
               seq_t; it simply increments by one. */
            cur = cur + 1;
        }
    }

    /* Compute a(n) = (n‑th nonprime) + n.
       Convert n to seq_t for the addition. */
    seq_t result = nonprimes[n - 1] + (seq_t)n;
    return result;
}
