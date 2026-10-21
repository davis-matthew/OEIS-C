/*
 * A083832  Palindromes of the form 4n + 1 where n is also a palindrome.
 *          Palindromes arising in A083831.
 * https://oeis.org/A083832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083832:
   a(n) = the n-th number of the form 4*m + 1 that is a palindrome 
   where m itself is a palindrome.  The starts: 5,9,33,353,525,...
   The implementation enumerates m as palindromes in ascending order, 
   substitutes 4*m+1, checks for palindromicity of the result, and 
   stores the accepted values.  Cached values allow repeated calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>   /* for realloc */

static bool is_pal(seq_t x) {
    seq_t rev = 0;
    seq_t t   = x;
    while (t > 0) {
        rev = rev * 10 + (t % 10);
        t   = t / 10;
    }
    return rev == x;
}

seq_t a(idx_t n) {
    /* Cache of found terms */
    static seq_t *terms = NULL;
    static size_t num_terms = 0;
    static idx_t last_checked = 1;   /* next candidate m after the last stored */

    if (n == 0) return 0;   /* invalid index, return 0 for safety */

    /* Grow cache if necessary */
    if (num_terms < n) {
        /* Ensure space for at least n terms */
        size_t new_cap = (num_terms == 0) ? 64 : (size_t)((double)num_terms * 1.5) + 64;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        terms = tmp;

        /* Generate terms until we have at least n of them */
        for ( ; num_terms < n; ++last_checked) {
            /* m must be a palindrome */
            if (!is_pal((seq_t)last_checked)) continue;

            seq_t val = 4 * (seq_t)last_checked + 1;
            if (is_pal(val)) {
                terms[num_terms++] = val;
            }
        }
    }
    return terms[n - 1];
}
