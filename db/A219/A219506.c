/*
 * A219506  Pierce expansion of 2 - sqrt(3).
 * https://oeis.org/A219506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 3, a(1) = 5
 * a(2*n)   = a(2*n-2)^3 + 3*a(2*n-2)^2 - 3
 * a(2*n+1) = a(2*n-1)^3 - 3*a(2*n-1)^2 + 3
 * (OEIS A219506)
 * The implementation memoises even and odd terms separately in dynamic
 * arrays that grow on demand, using only standard C allocation.
 */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n) {
    /* Static storage for even (a(0), a(2), a(4), ...) and odd (a(1), a(3), ...) values. */
    static seq_t *even_arr = NULL;
    static size_t  even_len = 0;   /* number of even terms stored */
    static size_t  even_cap = 0;   /* capacity of even_arr */

    static seq_t *odd_arr = NULL;
    static size_t  odd_len = 0;    /* number of odd terms stored */
    static size_t  odd_cap = 0;    /* capacity of odd_arr */

    /* Initialise the two arrays if this is the first call. */
    if (even_arr == NULL) {
        even_cap = 1;
        even_arr = (seq_t *)realloc(even_arr, even_cap * sizeof(seq_t));
        even_arr[0] = 3;          /* a(0) */
        even_len = 1;
    }
    if (odd_arr == NULL) {
        odd_cap = 1;
        odd_arr = (seq_t *)realloc(odd_arr, odd_cap * sizeof(seq_t));
        odd_arr[0] = 5;           /* a(1) */
        odd_len = 1;
    }

    if ((n & 1) == 0) {          /* Even n: index 2*i. */
        size_t target = (size_t)(n >> 1);  /* i = n/2 */
        if (target >= even_len) {
            for (size_t i = even_len; i <= target; ++i) {
                /* Grow the array if necessary. */
                if (i >= even_cap) {
                    size_t new_cap = (even_cap == 0) ? 1 : (even_cap << 1);
                    while (new_cap <= i) new_cap <<= 1;
                    even_arr = (seq_t *)realloc(even_arr, new_cap * sizeof(seq_t));
                    even_cap = new_cap;
                }
                if (i == 0) continue; /* first element already set */
                seq_t prev = even_arr[i - 1];
                seq_t sq   = prev * prev;
                seq_t cube = sq * prev;
                even_arr[i] = cube + 3 * sq - (seq_t)3;
            }
            even_len = target + 1;
        }
        return even_arr[target];
    } else {                    /* Odd n: index 2*i+1. */
        size_t target = (size_t)((n - 1) >> 1);  /* i = (n-1)/2 */
        if (target >= odd_len) {
            for (size_t i = odd_len; i <= target; ++i) {
                if (i >= odd_cap) {
                    size_t new_cap = (odd_cap == 0) ? 1 : (odd_cap << 1);
                    while (new_cap <= i) new_cap <<= 1;
                    odd_arr = (seq_t *)realloc(odd_arr, new_cap * sizeof(seq_t));
                    odd_cap = new_cap;
                }
                if (i == 0) continue; /* first element already set */
                seq_t prev = odd_arr[i - 1];
                seq_t sq   = prev * prev;
                seq_t cube = sq * prev;
                odd_arr[i] = cube - 3 * sq + (seq_t)3;
            }
            odd_len = target + 1;
        }
        return odd_arr[target];
    }
}
