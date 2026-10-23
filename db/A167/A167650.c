/*
 * A167650  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1}^{14} 3·a(n-k) – 6·a(n‑15) + coeff_N(n), with
 *            coeff_N(0)=1, coeff_N(1..14)=2, coeff_N(15)=1, coeff_N(n)=0 for
 *            n>15. This linear recurrence is derived from the rational
 *            generating function in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *buf       = NULL;  /* cached values a[0..last_computed] */
    static idx_t buf_cap    = 0;     /* allocated size of buf              */
    static idx_t last_computed = 0;  /* highest index already computed     */

    /* ---- allocate / grow buffer ------------------------------------ */
    if (buf == NULL) {
        buf       = seq_alloc_array(1);
        buf_cap   = 1;
        last_computed = 0;
    } else if (buf_cap <= n) {
        seq_t *newbuf = seq_alloc_array(n + 1);
        for (idx_t i = 0; i <= last_computed; ++i) newbuf[i] = buf[i];
        free_seq_array(buf);
        buf       = newbuf;
        buf_cap   = n + 1;
    }

    /* ---- initialise a[0] ------------------------------------------- */
    if (last_computed < 1) {
        buf[0] = (seq_t)1;
        last_computed = 0;
    }

    /* ---- compute all entries up to n ------------------------------ */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Σ_{k=1}^{14} a(i-k) */
        for (idx_t k = 1; k <= 14; ++k)
            if (i >= k) sum += buf[i - k];

        seq_t val = 3 * sum;
        if (i >= 15) val -= 6 * buf[i - 15];

        /* add numerator coefficient */
        seq_t add = 0;
        if (i <= 15) {
            if (i == 0 || i == 15)
                add = 1;
            else
                add = 2;
        }
        val += add;

        buf[i] = val;
    }

    last_computed = n;
    return buf[n];
}
