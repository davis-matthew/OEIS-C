/*
 * A049125  Revert transform of (1 + x - x^2) / (1 + x)^2.
 * https://oeis.org/A049125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=4 a(n) for n≥5 satisfies 5 n (n‑1)
 *            (37 n‑106) a(n) -4 (n‑1) (74 n²‑323 n+288) a(n‑1) +16 (‑74
 *            n³+508 n²‑1157 n+876) a(n‑2) +2 (2 n‑5) (37 n‑69) (n‑4) a(n‑3) =
 *            0 (OEIS A049125, D‑finite recurrence)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;   /* 0‑unused, indices start at 1 */
    static size_t cap     = 0;      /* allocated size (in seq_t units) */
    static idx_t  n_max   = 0;      /* largest n for which cache[n] is defined */

    /* Handle offset: for n<1 return 0 (value not used by callers) */
    if (n == 0) return (seq_t)0;

    /* Ensure capacity for index n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base terms once */
    if (n_max == 0) {
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)2;   /* a(3) */
        cache[4] = (seq_t)4;   /* a(4) */
        n_max   = 4;
    }

    /* Compute terms up to n using the recurrence */
    idx_t start = (n_max > 4 ? n_max + 1 : 5);   /* recurrence needs n≥5 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        /* Coefficients of the recurrence (all computed as seq_t) */
        seq_t P1 = (seq_t)5 * i_seq * (i_seq - 1) *
                   ((seq_t)37 * i_seq - (seq_t)106);

        seq_t P2 = -(seq_t)4 * (i_seq - 1) *
                   (((seq_t)74 * i_seq * i_seq) -
                    (seq_t)323 * i_seq +
                    (seq_t)288);

        seq_t P3 = (seq_t)16 *
                   ((((seq_t)-74 * i_seq * i_seq * i_seq) +
                     (seq_t)508 * i_seq * i_seq -
                     (seq_t)1157 * i_seq +
                     (seq_t)876));

        seq_t P4 = (seq_t)2 * ((seq_t)(2 * i_seq - 5)) *
                   ((seq_t)37 * i_seq - (seq_t)69) *
                   (i_seq - 4);

        /* Numerator: -(P2*a(i-1)+P3*a(i-2)+P4*a(i-3)) */
        seq_t num = -(P2 * cache[i - 1] +
                      P3 * cache[i - 2] +
                      P4 * cache[i - 3]);

        cache[i] = num / P1;   /* exact integer division */
    }

    n_max = n;
    return cache[n];
}
