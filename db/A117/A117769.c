/*
 * A117769  Lucas numbers for which the product of the digits is a Fibonacci
 *          number.
 * https://oeis.org/A117769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Lucas number L_k for the n-th Lucas number such that *
 *            digit_product(L_k) ∈ {0,1,2,3,5,8,21,144} (OEIS A117769)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 23, u64 n <= 60, u128 n <= 146,
 *            big any n
 * Helpers:   digit_product, fib_product_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/fib_product_p.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Lazy static initialization of state. */
    static seq_t *found = NULL;      /* array of Lucas numbers that satisfy the criterion */
    static idx_t found_len = 0;      /* number of stored terms             */
    static idx_t found_cap = 0;      /* capacity of the array              */

    /* Last two Lucas numbers used to generate the next one. */
    static seq_t l_prev = (seq_t)1;  /* L1 = 1 */
    static seq_t l_curr = (seq_t)3;  /* L2 = 3 */
    static bool initialized = false;

    /* Initialize storage and the first two qualifying terms. */
    if (!initialized) {
        found_cap = 16;
        found = (seq_t *)malloc(found_cap * sizeof(seq_t));
        if (!found) return 0;            /* memory allocation failed */
        found[0] = (seq_t)1;             /* L1 */
        found[1] = (seq_t)3;             /* L2 (digit product = 3) */
        found_len = 2;
        initialized = true;
    }

    /* Generate Lucas numbers until we have found n terms. */
    while (found_len < n) {
        /* Next Lucas number: L_{k+1} = L_{k-1} + L_k */
        seq_t next = l_prev + l_curr;
        l_prev = l_curr;
        l_curr = next;

        /* Check the digit product condition. */
        seq_t prod = digit_product(next);
        if (fib_product_p(prod)) {
            /* Store the qualifying Lucas number. */
            if (found_len >= found_cap) {
                found_cap <<= 1; /* double capacity */
                seq_t *tmp = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
                if (!tmp) { free(found); return 0; }
                found = tmp;
            }
            found[found_len++] = next;
        }
    }

    /* Return the requested term. */
    return found[n - 1];
}
