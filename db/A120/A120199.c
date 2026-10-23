/*
 * A120199  a(1)=6; a(n)=floor((53+sum(a(1) to a(n-1)))/8).
 * https://oeis.org/A120199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 79, u32 n <= 173, u64 n <= 362,
 *            u128 n <= 737, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 6;
   a(n) = floor((53 + Σ_{i=1}^{n‑1} a(i)) / 8)   for n ≥ 2   (OEIS A120199) */

#include <stdlib.h>    /* for malloc, realloc */

static seq_t *values = NULL;         /* values[0] = a(1) */
static idx_t   cached  = 0;          /* number of values currently stored */
static unsigned __int128 sum128 = 0; /* cumulative sum for 64‑bit builds */
static seq_t    sum_big = 0;          /* cumulative sum for big‑int builds */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n < 1 */

    if (cached < n) {                     /* need to extend cache */
        idx_t new_cap = cached ? cached : 1;
        while (new_cap < n) new_cap *= 2;

        if (new_cap > cached) {
            values = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        }

        for (idx_t i = cached; i < n; ++i) {
            seq_t next;

            if (sizeof(seq_t) > 8) {     /* arbitrary‑precision seq_t */
                next = (sum_big + (seq_t)53) / (seq_t)8;
                sum_big += next;         /* keep S_{i+1} */
            } else {                    /* 64‑bit or smaller seq_t */
                unsigned __int128 tmp = (sum128 + (unsigned __int128)53) / 8;
                next = (seq_t)tmp;
                sum128 += (unsigned __int128)next;
            }

            values[i] = next;
        }

        cached = n;
    }

    return values[n - 1];
}
