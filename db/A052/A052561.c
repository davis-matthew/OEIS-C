/*
 * A052561  a(n) = (1 + 2^n) * n!.
 * https://oeis.org/A052561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + 2^n) * n! (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Memoisation of already computed terms */
    static idx_t current_max = 0;          /* highest n computed so far */
    static seq_t *arr = NULL;              /* array of a(0) … a(current_max)   */
    static size_t cap = 0;                 /* allocated capacity (elements)     */
    static seq_t fact = 1;                 /* factorial of current_max           */
    static seq_t pow2 = 1;                 /* 2^current_max                     */

    /* Initialise on first call */
    if (arr == NULL) {
        cap = 1;
        arr = alloc_seq_array(cap);
        if (!arr) abort();                /* irrecoverable allocation failure */
        arr[0] = (seq_t)2;                /* (1 + 2^0) * 0! = 2 */
        current_max = 0;
        fact = (seq_t)1;                  /* 0! = 1  */
        pow2 = (seq_t)1;                  /* 2^0 = 1 */
    }

    /* If the requested index is beyond what we have, compute further */
    if (n > current_max) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_arr = alloc_seq_array(new_cap);
        if (!new_arr) abort();

        /* Copy terms that are already known */
        for (idx_t i = 0; i <= current_max; ++i)
            new_arr[i] = arr[i];

        /* Compute new terms */
        for (idx_t i = current_max + 1; i <= n; ++i) {
            fact = fact * (seq_t)i;        /* i! = (i–1)! * i */
            pow2 = pow2 * (seq_t)2;        /* 2^i = 2^(i–1) * 2 */
            new_arr[i] = (seq_t)1 + pow2;  /* (1 + 2^i)   */
            new_arr[i] = new_arr[i] * fact;/* (1 + 2^i) * i! */
        }

        /* Replace old array */
        free_seq_array(arr);
        arr = new_arr;
        cap = new_cap;
        current_max = n;
    }

    return arr[n];
}
