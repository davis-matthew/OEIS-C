/*
 * A063360  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          87 ).
 * https://oeis.org/A063360
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   The sequence satisfies the rational generating function G(x) =
 *            P(x)/Q(x) with P(x) = x^11 + 24x^10 + 211x^9 + 209x^8 + 237x^7 +
 *            393x^6 + 209x^5 + 211x^4 + 182x^3 - x^2, Q(x) = 1 - x + x^3 -
 *            2x^4 + x^5 - x^7 + x^8. Hence the coefficients a(n) obey a(n) =
 *            a(n‑1) – a(n‑3) + 2·a(n‑4) – a(n‑5) + a(n‑7...
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utility definitions for dynamic array handling */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *seq = NULL;      /* pointer to computed values */
static idx_t  seq_len = 0;     /* number of computed values (size of seq array) */

/* Pre‑computed p(n) values for 0 ≤ n ≤ 11; zero for larger n. */
static const seq_t p_val[12] = {
    (seq_t)0,   (seq_t)0,   (seq_t)-1,  (seq_t)182,
    (seq_t)211, (seq_t)209, (seq_t)393, (seq_t)237,
    (seq_t)209, (seq_t)211, (seq_t)24,  (seq_t)1
};

/* Ensure the array is computed up to index n, extending if necessary. */
static void ensure_computed(idx_t n)
{
    if (seq_len > n) return;          /* already available */

    /* Allocate new larger array. */
    seq_t *newseq = seq_alloc_new_array(n + 1);
    if (!newseq) return;              /* allocation failure – should not happen */

    /* Copy existing values if any. */
    if (seq) {
        memcpy(newseq, seq, sizeof(seq_t) * seq_len);
        seq_free_array(seq);
    }

    /* Compute new values from seq_len to n. */
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 1) val += newseq[i - 1];
        if (i >= 3) val -= newseq[i - 3];
        if (i >= 4) val += (seq_t)2 * newseq[i - 4];
        if (i >= 5) val -= newseq[i - 5];
        if (i >= 7) val += newseq[i - 7];
        if (i >= 8) val -= newseq[i - 8];
        if (i <= 11) val += p_val[i];
        newseq[i] = val;
    }

    seq = newseq;
    seq_len = n + 1;
}

seq_t a(idx_t n)
{
    /* The function is defined for n ≥ 2.  For smaller n the
       behaviour is unspecified, but we still return a computed
       value if available. */
    ensure_computed(n);
    return seq[n];
}
