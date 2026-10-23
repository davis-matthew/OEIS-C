/*
 * A126403  Number of base 16 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑16 n‑digit strings * where adjacent digits
 *            differ by at most 2. * * Recurrence: * f(1, d) = 1 (for every
 *            digit d=0..15) * f(k+1, d) = Σ_{i=max(0,d-2)}^{min(15,d+2)}
 *            f(k,i) * a(k) = Σ_{d=0}^{15} f(k,d) * a(0) = 1 * * The function
 *            memoises previous results for fast repeated calls
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *seq = NULL;      /* array of computed a(k) values */
    static size_t seq_cap = 0;     /* allocated capacity of seq    */
    static size_t seq_len = 0;     /* number of values stored (len) */
    static seq_t counts[16];       /* f(current_len, d)  */
    static int counts_valid = 0;   /* 1 if counts holds f(seq_len-1, d) */

    /* initialise a(0) on first call */
    if (seq_len == 0) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 1;
        seq_len = 1;          /* now a(0) is stored */
        counts_valid = 0;     /* counts not yet initialised */
    }

    /* return already stored value */
    if ((idx_t)n < seq_len)
        return seq[n];

    /* handle a(1) specially (jump from a(0) to a(1)) */
    if (seq_len == 1 && n >= 1) {
        if (!counts_valid) {
            for (int d = 0; d < 16; ++d)
                counts[d] = 1; /* f(1,d) = 1 */
            counts_valid = 1;
        }
        seq_t term = 0;
        for (int d = 0; d < 16; ++d)
            term += counts[d]; /* term = a(1) = 16 */

        /* grow array if necessary */
        if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq[seq_len] = term; /* store a(1) */
        seq_len++;           /* now seq_len == 2 */

        /* after storing a(1), counts already hold f(1,d) */
        if (n == 1)
            return seq[1];
    }

    /* compute subsequent terms incrementally */
    while (seq_len <= n) {
        seq_t next_counts[16];
        for (int d = 0; d < 16; ++d) {
            seq_t s = 0;
            int lo = d - 2;
            if (lo < 0) lo = 0;
            int hi = d + 2;
            if (hi > 15) hi = 15;
            for (int i = lo; i <= hi; ++i)
                s += counts[i];
            next_counts[d] = s;
        }
        seq_t term = 0;
        for (int d = 0; d < 16; ++d)
            term += next_counts[d];

        /* grow array if needed */
        if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap * 2;
            if (new_cap == 0) new_cap = 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq[seq_len] = term;
        seq_len++;

        /* update counts for next iteration */
        for (int d = 0; d < 16; ++d)
            counts[d] = next_counts[d];
    }

    return seq[n];
}
