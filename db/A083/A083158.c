/*
 * A083158  Palindromes which are the arithmetic mean of two distinct
 *          palindromes.
 * https://oeis.org/A083158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th palindrome that can be expressed as the
 *            arithmetic mean of two distinct palindromes (base 10)
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

/* dynamic array of previously seen palindromes (ascending) */
static seq_t *pal_arr = NULL;
static size_t pal_size = 0, pal_cap = 0;

/* dynamic array of sequence values a(1), a(2), … */
static seq_t *seq_arr = NULL;
static size_t seq_size = 0, seq_cap = 0;

/* last integer examined */
static idx_t last_cur = 0;

/* Grow a dynamic array, used for both pal_arr and seq_arr */
static void grow_array(seq_t **arr, size_t *size, size_t *cap)
{
    if (*size == *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();        /* memory error, terminate */
        *arr = tmp;
        *cap = newcap;
    }
}

/* Main sequence function: returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset irrelevant */

    /* compute at least n terms, memoize results */
    while (seq_size < n) {
        last_cur++;                   /* next integer to test        */
        seq_t cur_seq = (seq_t)last_cur;

        /* skip non‑palindromes */
        if (!is_palindrome(cur_seq, (idx_t)10))
            continue;

        /* check if cur_seq can be written as (p + q)/2 with distinct palindromes p,q */
        seq_t found = 0;
        seq_t sum = cur_seq + cur_seq;        /* 2 * cur_seq */
        for (size_t i = 0; i < pal_size; ++i) {
            seq_t p = pal_arr[i];
            /* b = 2*cur - p */
            seq_t b = sum - p;
            if (b != p && is_palindrome(b, (idx_t)10)) {
                found = 1;
                break;
            }
        }

        /* add current palindrome to the list of palindromes */
        grow_array(&pal_arr, &pal_size, &pal_cap);
        pal_arr[pal_size++] = cur_seq;

        /* if found, store in the sequence */
        if (found) {
            grow_array(&seq_arr, &seq_size, &seq_cap);
            seq_arr[seq_size++] = cur_seq;
        }
    }

    return seq_arr[n - 1];
}
