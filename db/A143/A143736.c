/*
 * A143736  Numbers n such that if f[n] = n-th Fibonacci number then
 *          f[n]-n=prime and f[n]+n=prime.
 * https://oeis.org/A143736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = F_k where k is such that F_k - k and F_k + k are both
 *            prime. The function computes the nth such Fibonacci number,
 *            using a memoized search over increasing indices k
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* Return the n-th Fibonacci number that satisfies the condition
   F_k - k and F_k + k are prime. */
seq_t a(idx_t n) {
    /* Static cache of found values */
    static seq_t *values = NULL;   /* array of seq_t */
    static size_t values_cap = 0;  /* capacity of array */
    static size_t values_len = 0;  /* number of stored values */

    /* Fibonacci state (for incremental computation) */
    static seq_t f_prev = 0;        /* F_{k-1} */
    static seq_t f_curr = 1;        /* F_k   */
    static idx_t next_k = 1;        /* next index to test (k) */

    /* Initialize only once */
    static bool initialized = false;
    if (!initialized) {
        f_prev = 0;
        f_curr = 1;
        next_k = 1;
        initialized = true;
    }

    /* Keep computing until we have at least n values */
    while (values_len < (size_t)n) {
        /* Check the current Fibonacci number */
        seq_t minus = f_curr - (seq_t)next_k;
        seq_t plus  = f_curr + (seq_t)next_k;

        if (is_prime_big(minus) && is_prime_big(plus)) {
            /* Store this Fibonacci number */
            grow_seq(&values, &values_cap, values_len + 1);
            values[values_len++] = f_curr;
        }

        /* Advance Fibonacci sequence to the next index */
        seq_t tmp = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = tmp;
        next_k++;
    }

    return values[n - 1];
}
