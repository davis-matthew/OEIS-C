/*
 * A039965  An example of a d-perfect sequence.
 * https://oeis.org/A039965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^(n+1) * Catalan(n-1)) mod 3
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Catalan numbers modulo 3 – memoised */
    static seq_t *cat = NULL;          /* cat[0] = 1 (Catalan 0) */
    static size_t cat_cap = 0;         /* allocated length of cat array   */
    static idx_t cat_max = 0;          /* highest index computed (in 0‑based) */

    /* Index of the Catalan we need */
    if (n == 0) return 0; /* out of range, but safe */
    idx_t idx = n - 1;                 /* we need Catalan(n-1) */

    /* initialise storage if first call */
    if (!cat) {
        cat_cap = 1;
        cat = (seq_t*)malloc(cat_cap * sizeof(seq_t));
        cat[0] = (seq_t)1;            /* Catalan(0) = 1 */
        cat_max = 0;
    }

    /* grow the array if necessary */
    if (idx > cat_max) {
        if (cat_cap <= (size_t)idx) {
            size_t new_cap = cat_cap > 0 ? cat_cap : 1;
            while (new_cap <= (size_t)idx) new_cap <<= 1;
            cat = (seq_t*)realloc(cat, new_cap * sizeof(seq_t));
            cat_cap = new_cap;
        }
        /* compute missing Catalan modulo 3 values */
        for (idx_t k = cat_max + 1; k <= idx; ++k) {
            int sum = 0;
            for (idx_t i = 0; i < k; ++i) {
                sum += (int)cat[i] * (int)cat[k - 1 - i];
            }
            cat[k] = (seq_t)(sum % 3);
        }
        cat_max = idx;
    }

    /* retrieve Catalan(n-1) modulo 3 */
    int val = (int)cat[idx];           /* 0,1,2 */

    /* apply the sign (-1)^(n+1) */
    if ((n % 2) == 0) {                /* n even -> sign = -1 */
        if (val != 0) val = 3 - val;   /* 1->2, 2->1, 0->0 */
    }
    return (seq_t)val;
}
