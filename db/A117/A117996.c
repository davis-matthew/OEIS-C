/*
 * A117996  Padovan numbers with only odd digits.
 * https://oeis.org/A117996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   OEIS A117996: Padovan numbers having only odd digits.
   The sequence is obtained by generating Padovan numbers 
   (P(0)=P(1)=P(2)=1, P(n)=P(n-2)+P(n-3) for n≥3) in order, 
   discarding any value that contains an even digit, and
   keeping each distinct value only once (duplicates such as the
   initial “1” and “2” appear only once).  The returned term
   a(n) corresponds to the n‑th distinct odd‑digit Padovan number
   (a(1)=1, a(2)=3, a(3)=5, …).  The function caches computed
   terms so subsequent calls are fast.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"

static bool odd_digits(seq_t x)
{
    /* Need at least the maximum number of decimal digits that may occur
       for all computed Padovan numbers.  A generous upper bound of 50 000
       digits keeps the buffer far above what is required for n≤10 000. */
    static uint8_t buf[50000];
    size_t len;
    get_digits(x, buf, &len);          /* fill buf with decimal digits, least‑significant first */
    for (size_t i = 0; i < len; ++i)
        if ((buf[i] & 1) == 0)         /* even digit? */
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t last_output = (seq_t)-1;          /* last distinct odd‑digit Padovan number output */
    static seq_t arr[3];                          /* arr[0]=P_{i-3}, arr[1]=P_{i-2}, arr[2]=P_{i-1} */
    static bool init_done = false;

    /* ----------- initialisation ----------------------------------- */
    if (!init_done) {
        init_done = true;
        terms_cap = 32;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;              /* abort on allocation failure */

        /* seed the Padovan recurrence with the known initial values 1,1,1 */
        seq_t first[3] = { (seq_t)1, (seq_t)1, (seq_t)1 };
        for (int i = 0; i < 3; ++i) {
            seq_t v = first[i];
            if (v != last_output && odd_digits(v)) {
                if (terms_len >= terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = v;
                last_output = v;
            }
        }
        /* initialise recurrence state */
        arr[0] = first[0];
        arr[1] = first[1];
        arr[2] = first[2];
    }

    /* ----------- extend cache until we have enough terms ---------- */
    while (terms_len < n) {
        seq_t next = arr[0] + arr[1];      /* P_{i} = P_{i-2} + P_{i-3} */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = next;

        if (next != last_output && odd_digits(next)) {
            if (terms_len >= terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = next;
            last_output = next;
        }
    }

    return terms[n - 1];
}
