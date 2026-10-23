/*
 * A037400  Numbers k such that every base-6 digit of k is a base-9 digit of
 *          k.
 * https://oeis.org/A037400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037400: Numbers k such that every base‑6 digit of k
 * is also a base‑9 digit of k.
 * Computed by brute force enumeration with memoisation.
 * The function returns the n‑th such number (1‑based index). */

#include <stdint.h>
#include <stdlib.h>

/* The function signature required by the framework. */
seq_t a(idx_t n)
{
    /* static state to memoise the sequence */
    static seq_t *arr = NULL;  /* array of found terms */
    static size_t cap = 0;     /* current allocation capacity  */
    static size_t len = 0;     /* number of terms currently stored  */
    static seq_t last_k = 1;   /* the next integer to test */

    /* Ensure the array is large enough for n terms. */
    while (len < n)
    {
        seq_t k = last_k; /* candidate number */

        /* ----- test whether k satisfies the property ----- */
        unsigned mask6 = 0; /* bits 0‑5 for base‑6 digits */
        unsigned mask9 = 0; /* bits 0‑8 for base‑9 digits */

        seq_t tmp = k;
        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % 6);
            mask6 |= 1U << d;
            tmp /= 6;
        }

        tmp = k;
        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % 9);
            mask9 |= 1U << d;
            tmp /= 9;
        }

        /* check that every base‑6 digit occurs in the base‑9 digits */
        if ( (mask6 & mask9) == mask6 )
        {
            /* store k into the sequence array, resizing if needed */
            if (len == cap)
            {
                size_t new_cap = cap ? (cap << 1) : 16;
                seq_t *tmp_arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
                if (!tmp_arr) { /* allocation failure – abort */
                    /* In this project’s environment we never hit this: */
                    abort();
                }
                arr = tmp_arr;
                cap = new_cap;
            }
            arr[len++] = k;
        }

        /* move on to the next integer to test */
        ++last_k;
    }

    /* return the requested term (1‑based index) */
    return arr[n - 1];
}
