/*
 * A071249  Numbers k such that gcd(k, R(k)) > 1, where R(k) (A004086) is the
 *          digit reversal of k.
 * https://oeis.org/A071249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 5727, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071249: the n-th integer k (k ≥ 2) such that gcd(k, R(k)) > 1,
   where R(k) is the decimal digit reversal of k.  The function
   computes terms by direct search, caching results for subsequent calls. */

#include <stdlib.h>

/* Helper: reverse decimal digits of a number. */
static seq_t reverse_digits(seq_t v) {
    seq_t rev = 0;
    while (v > (seq_t)0) {
        rev = rev * (seq_t)10 + (v % (seq_t)10);
        v = v / (seq_t)10;
    }
    return rev;
}

/* Helper: Euclidean GCD for seq_t values. */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static storage for memoisation. */
    static seq_t *terms = NULL;          /* array of found terms      */
    static size_t len   = 0;             /* number of terms stored   */
    static size_t cap   = 0;             /* current array capacity   */
    static idx_t  next_k = (idx_t)2;     /* next integer to test     */

    /* Initialise storage on first call. */
    if (terms == NULL) {
        cap   = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        seq_t k    = (seq_t)next_k;
        seq_t rev  = reverse_digits(k);
        seq_t g    = gcd_seq(k, rev);
        if (g > (seq_t)1) {
            if (len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = k;
        }
        next_k++;
    }

    return terms[n - 1];
}
