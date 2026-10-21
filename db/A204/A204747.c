/*
 * A204747  Number of (n+2)X3 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically, diagonally or
 *          antidiagonally exactly three ways, and new values 0..1 introduced
 *          in row major order.
 * https://oeis.org/A204747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - 10*a(n-4) + 7*a(n-5) +
 *            7*a(n-6) -17*a(n-7) + 6*a(n-8) + 6*a(n-9) + 4*a(n-10) -
 *            2*a(n-11) + 4*a(n-12) -16*a(n-13) + 8*a(n-14) (for n > 15)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 32, u64 n <= 70, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (n = 1 … 15) are listed below. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 15 terms of the sequence */
    static const seq_t init[15] = {
        40ULL,
        61ULL,
        102ULL,
        188ULL,
        342ULL,
        590ULL,
        1051ULL,
        1915ULL,
        3308ULL,
        5967ULL,
        10934ULL,
        19151ULL,
        34428ULL,
        62824ULL,
        110673ULL
    };

    /* Lazy, resizable cache of all computed terms (a(1) at index 0) */
    static seq_t *terms = NULL;
    static idx_t terms_len = 0;   /* number of terms currently stored      */
    static size_t terms_cap = 0;  /* current allocation size (in elements) */

    /* Handle the trivial case n <= 15 */
    if (n <= 15U) {
        return init[n - 1];
    }

    /* Initialize the cache on first call */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return 0;          /* allocation failure */
        for (idx_t i = 0; i < 15U; ++i) {
            terms[i] = init[i];
        }
        terms_len = 15U;
    }

    /* Extend the cache as needed */
    while (terms_len < n) {
        idx_t next_index = terms_len + 1;   /* 1‑based index of the new term */

        /* Compute using the empirical recurrence.
           Mapping: a(k) is stored at terms[k-1] (0‑based).  */
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive weighted terms */
        pos += terms[next_index - 2] * (seq_t)3;      /* a(n-1) * 3 */
        pos += terms[next_index - 4] * (seq_t)3;      /* a(n-3) * 3 */
        pos += terms[next_index - 6] * (seq_t)7;      /* a(n-5) * 7 */
        pos += terms[next_index - 7] * (seq_t)7;      /* a(n-6) * 7 */
        pos += terms[next_index - 9] * (seq_t)6;      /* a(n-8) * 6 */
        pos += terms[next_index -10] * (seq_t)6;      /* a(n-9) * 6 */
        pos += terms[next_index -11] * (seq_t)4;      /* a(n-10)* 4 */
        pos += terms[next_index -13] * (seq_t)4;      /* a(n-12)* 4 */
        pos += terms[next_index -15] * (seq_t)8;      /* a(n-14)* 8 */

        /* Negative weighted terms */
        neg += terms[next_index - 3] * (seq_t)2;      /* a(n-2) * 2     */
        neg += terms[next_index - 5] * (seq_t)10;     /* a(n-4) *10     */
        neg += terms[next_index - 8] * (seq_t)17;     /* a(n-7) *17     */
        neg += terms[next_index -12] * (seq_t)2;      /* a(n-11)* 2     */
        neg += terms[next_index -14] * (seq_t)16;     /* a(n-13)*16     */

        seq_t val = pos - neg;           /* all intermediate values are non‑negative */

        /* Resize if necessary */
        if (terms_len >= terms_cap) {
            size_t new_cap = terms_cap * 2;
            seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) {
                /* allocation failure – abort gracefully */
                return 0;
            }
            terms = new_terms;
            terms_cap = new_cap;
        }

        terms[terms_len] = val;
        terms_len++;
    }

    return terms[n - 1];
}
