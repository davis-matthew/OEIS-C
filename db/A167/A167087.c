/*
 * A167087  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,
   a(n)=32*(a(n-1)+a(n-2)+…+a(n-12))−528*a(n-13) for n≥14,
   a(13)=32*(a12+…+a1)−528*a0+1.
   (The first 13 terms are derived from the generating function.) */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t len = 0;          /* number of terms already stored */

    /* lazy initialization */
    if (seq == NULL) {
        cap = 16;                   /* initial capacity > 14 */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;         /* allocation failure */
        len = 0;
    }

    /* grow the array to accommodate n */
    while (len <= n) {
        /* ensure capacity */
        if ((size_t)len >= cap) {
            size_t new_cap = cap ? cap * 2 : 1;
            while ((size_t)len >= new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;    /* allocation failure */
            seq = tmp;
            cap = new_cap;
        }

        /* compute next term */
        seq_t C32  = (seq_t)32;
        seq_t C528 = (seq_t)528;
        if (len == 0) {
            /* a(0) = 1 */
            seq[0] = (seq_t)1;
        }
        else if (len <= 12) { /* 1 ≤ n ≤ 12 */
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j < (size_t)len; ++j)
                sum += seq[j];
            seq[len] = sum * C32 + (seq_t)2;
        }
        else if (len == 13) { /* a(13) */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 12; ++j)
                sum += seq[13 - j];
            seq[13] = sum * C32 - seq[0] * C528 + (seq_t)1;
        }
        else { /* n ≥ 14 */
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 12; ++j)
                sum += seq[len - j];
            seq[len] = sum * C32 - seq[len - 13] * C528;
        }
        ++len;
    }

    return seq[n];
}
