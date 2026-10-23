/*
 * A176072  Primes of the form 3^k + 2^k + k + 1.
 * https://oeis.org/A176072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = the nth prime of the form 3^k + 2^k + k + 1 (OEIS
 *            A176072)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1, but safe for caller */

    /* Static state to cache computed terms. */
    static size_t capacity = 0;          /* allocated slots */
    static seq_t *values   = NULL;       /* array of found primes   */
    static idx_t  count    = 0;          /* primes stored so far   */
    static idx_t  next_k   = 1;          /* next exponent to test  */

    /* Initial allocation. */
    if (values == NULL) {
        capacity = 16;
        values   = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Find primes until we have at least n of them. */
    while (count < n) {
        /* Compute 3^k + 2^k + k + 1 using provided big power helper. */
        seq_t val = big_pow((seq_t)3, (idx_t)next_k);
        val += big_pow((seq_t)2, (idx_t)next_k);
        val += (seq_t)next_k;   /* add k   */
        val += (seq_t)1;        /* add 1   */

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Grow array if necessary. */
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
                if (tmp == NULL) {   /* allocation failure – abort */
                    free(values);
                    return 0;
                }
                values = tmp;
                capacity = new_cap;
            }
            values[count] = val;
            count++;
        }
        next_k++;
    }

    return values[n - 1];
}
