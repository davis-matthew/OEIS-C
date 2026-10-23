/*
 * A170279  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   a(0) = 1
   For 1 ≤ n ≤ 40:  a(n) = 2 + 28 * Σ_{k = n-1 downto max(0,n-40)} a(k)
   For n ≥ 41:      a(n) = 28 * Σ_{k = n-1 downto n-40} a(k) – 406 * a(n-41)
   This recurrence exactly reproduces OEIS A170279. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *data   = NULL;   /* stored sequence values  */
    static size_t cap    = 0;      /* allocated capacity     */
    static size_t len    = 0;      /* number of terms stored */
    static seq_t winSum  = 0;      /* sum of the last up to 40 terms */

    size_t target = (size_t)n;

    /* allocate (or grow) storage if necessary */
    if (len <= target) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= target) new_cap <<= 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            data = tmp;
            cap  = new_cap;
        }

        /* compute terms that are not yet stored */
        for (size_t i = len; i <= target; ++i) {
            seq_t coeff = 0;
            if (i == 0)            coeff = 1;
            else if (i <= 40)      coeff = 2;
            else if (i == 41)      coeff = 1;

            seq_t val = coeff;
            val += winSum * (seq_t)28;          /* add the 28‑multiple of the window sum */
            if (i >= 41)
                val -= data[i - 41] * (seq_t)406;

            data[i] = val;

            /* update sliding window of last up to 40 terms */
            winSum += val;
            if (i >= 40)
                winSum -= data[i - 40];
        }
        len = target + 1;
    }
    return data[(size_t)n];
}
