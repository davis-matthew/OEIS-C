/*
 * A109872  Palindromes such that successive difference of terms is also a
 *          palindrome.
 * https://oeis.org/A109872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(1)=1; for n>1, a(n) is the smallest positive integer greater
 *            than a(n‑1) that is a palindrome and whose difference
 *            a(n)-a(n‑1) is also a palindrome
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: full, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite, ending with a(36)=9119.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    /* Cache of generated terms */
    static seq_t *seq   = NULL;   /* array of terms                */
    static idx_t  size  = 0;      /* number of terms stored       */
    static idx_t  cap   = 0;      /* capacity of the array        */
    static idx_t  last  = 0;      /* last integer examined        */
    static bool   finished = false; /* true once all terms known */

    /* If we have already finished and n is beyond the known range,
       return the last known term. */
    if (finished && n > size) {
        return seq[size - 1];
    }

    /* Generate terms until we have at least n of them.  */
    while (size < n) {
        /* Increase the integer to test. */
        ++last;
        seq_t cand = (seq_t)last;

        /* Is cand a palindrome? */
        if (cand == reverse_digits(cand)) {
            /* First term or difference palindrome? */
            if (size == 0 || (cand - seq[size - 1]) == reverse_digits(cand - seq[size - 1])) {
                push_seq(&seq, &cap, &size, cand);

                /* Detect the final term (a(36)=9119).  */
                if (cand == (seq_t)9119) {
                    finished = true;
                }
            }
        }

        /* Prevent an infinite loop once the sequence is known to end. */
        if (finished && size < n) {
            /* No further terms exist; return the last known term. */
            return seq[size - 1];
        }
    }

    return seq[n - 1];
}
