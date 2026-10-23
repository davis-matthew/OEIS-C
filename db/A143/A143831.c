/*
 * A143831  Numbers n such that 12n^2 - 1 is prime.
 * https://oeis.org/A143831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that 12*m^2-1 is prime
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* static state for memoisation */
    static seq_t *pool   = NULL;   /* array of found numbers */
    static size_t stored = 0;      /* how many numbers are stored */
    static size_t cap    = 0;      /* current capacity of pool */
    static idx_t  last_checked = 0;/* last n that has been examined */

    while (stored < n) {
        ++last_checked;          /* next integer to test */

        /* compute 12*last_checked^2 - 1 in seq_t */
        seq_t val = (seq_t)last_checked * last_checked; /* m^2 */
        val *= (seq_t)12;
        val -= (seq_t)1;

        if (is_prime_big(val)) {
            /* grow the array if needed */
            if (stored == cap) {
                size_t new_cap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(pool, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                pool = tmp;
                cap = new_cap;
            }
            pool[stored++] = (seq_t)last_checked;
        }
    }
    return pool[n - 1];
}
