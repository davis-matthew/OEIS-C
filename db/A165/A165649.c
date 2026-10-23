/*
 * A165649  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32·(a(n‑1)+a(n‑2)+…+a(n‑8)) – 528·a(n‑9) + p(n)
 *            wherep(0)=1, p(1)=…=p(8)=2, p(9)=1, and p(n)=0 for n>9. This
 *            recurrence follows from the rational generating function (t^9 +
 *            2t^8 + … + 2t + 1) / (528t^9 – 32t^8 – … – 32t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* Recent numerator coefficients p(n) for n = 0..9 */
    static const unsigned char p_coeff[10] = {
        1, /* p(0) */
        2, 2, 2, 2, 2, 2, 2, 2, /* p(1)..p(8) */
        1  /* p(9) */
    };

    /* Static cache of previously computed values */
    static seq_t *buf   = NULL;
    static idx_t  next  = 0;      /* first index that still needs computation */
    static size_t cap   = 0;      /* allocated capacity */

    /* Allocate or enlarge the buffer as required */
    if (buf == NULL) {
        cap = (size_t)n + 1;
        if (cap < 9) cap = 9;     /* we will need at least the first nine terms */
        buf = alloc_seq(cap);
        next = 0;
    } else if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * cap) new_cap = 2 * cap;
        seq_t *new_buf = alloc_seq(new_cap);
        for (idx_t i = 0; i < next; ++i) new_buf[i] = buf[i];
        /* old buffer is intentionally leaked: it is harmless for the lifetime
           of this program (typically short‑lived test runs). */
        buf = new_buf;
        cap = new_cap;
    }

    /* Compute all missing terms up to index n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t sum = 0;

        /* Sum of a(i‑1)…a(i‑8) (only existing indices) */
        for (idx_t j = 1; j <= 8; ++j) {
            if (i >= j) sum += buf[i - j];
        }

        seq_t val;
        if (i >= 9) {
            seq_t p_val = 0;
            if (i == 9) p_val = (seq_t)p_coeff[9];
            val = sum * (seq_t)32
                - (seq_t)528 * buf[i - 9]
                + p_val;
        } else {
            /* For i < 9: no subtraction term, just the constant p(i) */
            val = sum * (seq_t)32 + (seq_t)p_coeff[i];
        }
        buf[i] = val;
    }
    next = n + 1;   /* all values up to n are now filled */

    return buf[n];
}
