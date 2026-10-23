/*
 * A176549  Primes of the form 2*n^2+6*n+1.
 * https://oeis.org/A176549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the primes of the form 2*n²+6*n+1, in increasing order
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Return the n-th prime of the form 2*m^2 + 6*m + 1. */
    static seq_t *primes = NULL;   /* array of found primes */
    static size_t cap   = 0;       /* capacity of the array */
    static idx_t last_n_checked = 0; /* last m that was inspected */
    static idx_t count   = 0;       /* number of primes stored */

    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    while (count < n) {
        ++last_n_checked;                               /* next m */

        /* Compute p = 2*m^2 + 6*m + 1 in seq_t arithmetic */
        seq_t m = (seq_t)last_n_checked;
        seq_t p = 2 * m * m + 6 * m + 1;

        /* Test primality; is_prime_big works for any seq_t size */
        if (is_prime_big(p)) {
            /* Grow the primes array if necessary */
            grow_seq(&primes, &cap, count + 1);
            primes[count++] = p;          /* store the new prime */
        }
    }

    return primes[n - 1];
}
