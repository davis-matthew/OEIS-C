/*
 * A169186  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c_n + 27*Σ_{k=1}^{26} a(n‑k) – 378*a(n‑27) where c_n = 1
 *            if n = 0 or n = 27, 2 if 1 ≤ n ≤ 26, else 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/grow_seq.h"
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* cached computed sequence */
    static size_t seq_cap = 0;         /* allocated capacity */
    static size_t seq_len = 0;         /* number of computed terms */

    /* initialize cache on first call */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = alloc_seq_array(seq_cap);
        seq[0] = (seq_t)1;            /* a(0) = 1 */
        seq_len = 1;
    }

    /* if requested term already computed, return it */
    if ((size_t)n < seq_len)
        return seq[n];

    /* compute missing terms up to n */
    for (idx_t i = seq_len; i <= n; ++i) {
        /* ----------------- compute coefficient c_i ----------------- */
        seq_t c_i = 0;
        if (i == 0 || i == 27)
            c_i = (seq_t)1;
        else if (i >= 1 && i <= 26)
            c_i = (seq_t)2;

        /* -------------- sum of the 26 previous terms -------------- */
        seq_t sum26 = 0;
        idx_t start = (i > 26) ? (i - 26) : 0;
        for (idx_t k = start; k < i; ++k)
            sum26 += seq[(size_t)k];

        /* -------------------- recurrence step --------------------- */
        seq_t next = c_i + seq_mul((seq_t)27, sum26);
        if (i >= 27) {
            seq_t sub = seq_mul((seq_t)378, seq[(size_t)(i - 27)]);
            next -= sub;
        }

        /* grow the buffer if needed */
        if (seq_len == seq_cap)
            grow_seq((seq_t **)&seq, &seq_cap, seq_cap * 2 + 1);

        seq[seq_len++] = next;
    }

    return seq[n];
}
