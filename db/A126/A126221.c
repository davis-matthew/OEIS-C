/*
 * A126221  a(n)=c(n)+c(n-1)+2*c(n-2)+4*c(n-3)+8*c(n-4)+...+2^(n-2)*c(1)+2^(n-1)*c(0),
 *          where c(k) are the Catalan numbers (A000108).
 * https://oeis.org/A126221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n)+c(n-1)+2*c(n-2)+4*c(n-3)+...+2^(n-1)*c(0), where
 *            c(k) are Catalan numbers (A000108)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- Cached Catalan numbers and powers of two --- */
    static seq_t *cat = NULL;         /* c[k] for 0..last_cat */
    static idx_t last_cat = (idx_t)-1;
    static seq_t *pow2 = NULL;        /* 2^k for 0..last_pow2 */
    static idx_t last_pow2 = (idx_t)-1;

    /* Ensure we have enough space */
    if (cat == NULL) {
        cat = (seq_t*)malloc(sizeof(seq_t) * 1);
        if (!cat) exit(EXIT_FAILURE);
        cat[0] = (seq_t)1;          /* c(0) */
        last_cat = 0;
    } else if (last_cat < n) {
        size_t needed = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cat, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cat = tmp;
        for (idx_t k = last_cat; k < n; ++k) {
            /* c(k+1) = c(k) * (4k+2) / (k+2) */
            seq_t num = cat[k] * (seq_t)(4 * k + 2);
            cat[k + 1] = num / (seq_t)(k + 2);
        }
        last_cat = n;
    }

    if (pow2 == NULL) {
        pow2 = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
        if (!pow2) exit(EXIT_FAILURE);
        pow2[0] = (seq_t)1;          /* 2^0 */
        last_pow2 = 0;
    } else if (last_pow2 < n) {
        size_t needed = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(pow2, needed * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        pow2 = tmp;
        for (idx_t i = last_pow2 + 1; i <= n; ++i) {
            pow2[i] = pow2[i - 1] * (seq_t)2;
        }
        last_pow2 = n;
    }

    /* ----- Compute a(n) ----- */
    seq_t result = cat[n];              /* term c(n) */
    if (n >= 1) result += cat[n - 1];   /* term c(n-1) */

    for (idx_t j = 2; j <= n; ++j) {
        /* coefficient = 2^(j-1) */
        seq_t coeff = pow2[j - 1];
        idx_t idx = n - j;
        result += coeff * cat[idx];
    }

    return result;
}
