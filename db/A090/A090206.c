/*
 * A090206  Nonprime Fibonacci numbers.
 * https://oeis.org/A090206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   allocate_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th nonprime Fibonacci number (including 0 and 1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/allocate_seq_array.h"

/* Allocate an array of seq_t suitable for both C and C++ */

/* Free an array allocated by allocate_seq_array */
static void free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Static state: cached composites and Fibonacci generator */
    static seq_t *composites = NULL;   /* array of nonprime Fibonacci numbers */
    static size_t cap     = 0;         /* allocated capacity */
    static size_t len     = 0;         /* number of composites stored */
    static seq_t f0 = 0;               /* current Fibonacci number */
    static seq_t f1 = 1;               /* next Fibonacci number */

    /* Generate Fibonacci numbers until we have at least n composites */
    while (len < n) {
        seq_t cur = f0;   /* current Fibonacci term */

        /* 0 and 1 are not prime, so include them; skip primes */
        if (!is_prime_big(cur)) {
            if (len == cap) {
                size_t new_cap = (cap == 0 ? 8 : cap * 2);
                seq_t *new_arr = allocate_seq_array(new_cap);
                for (size_t i = 0; i < len; ++i) new_arr[i] = composites[i];
                if (composites) free_seq_array(composites);
                composites = new_arr;
                cap = new_cap;
            }
            composites[len++] = cur;
        }

        /* Advance Fibonacci generator: f0, f1 = f1, f0+f1 */
        seq_t next = f0 + f1;
        f0 = f1;
        f1 = next;
    }

    return composites[n - 1];
}
