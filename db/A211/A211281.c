/*
 * A211281  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 2]
 *          as of [2, 3].
 * https://oeis.org/A211281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A211281
   a(n) = number of n‑letter words over the alphabet {1,2,3}
          having the same number of consecutive subwords “12” as “23”.
   The algorithm uses dynamic programming over the difference
   d = (#12) – (#23).  For a word of length ℓ, d can range from
   –(ℓ‑1) to +(ℓ‑1); thus all states fit into an array of size 2n+1.
   The method runs in O(n²) time and O(n) memory and works for every
   idx_t and seq_t type supplied by the framework. */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Return the value of a(n).  idx_t and seq_t are defined by    */
/* the build system.  The function has external linkage.        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n = 0 → empty word, counted once */
    if (n == 0)
        return (seq_t)1;

    /* Number of possible differences d for lengths up to n
       (index 0 … 2n, offset pointing to d==0) */
    const size_t size  = (size_t)(2 * n + 1);
    const size_t offset = (size_t)n;

    /* Allocate six arrays for the DP */
    seq_t *cur1 = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *cur2 = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *cur3 = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *tmp1 = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *tmp2 = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *tmp3 = (seq_t *)malloc(size * sizeof(seq_t));

    /* Safety: initialise all elements to zero */
    for (size_t i = 0; i < size; ++i) {
        cur1[i] = cur2[i] = cur3[i] = 0;
        tmp1[i] = tmp2[i] = tmp3[i] = 0;
    }

    /* Length‑1 words: all differences zero */
    cur1[offset] = 1;
    cur2[offset] = 1;
    cur3[offset] = 1;

    /* Transition for lengths 2 … n */
    for (idx_t len = 2; len <= n; ++len) {
        /* Valid range of differences for the current length */
        const size_t cur_start = offset - (size_t)(len - 1);
        const size_t cur_end   = offset + (size_t)(len - 1);

        /* Zero the temporary arrays only over the needed range */
        for (size_t i = cur_start; i <= cur_end; ++i) {
            tmp1[i] = tmp2[i] = tmp3[i] = 0;
        }

        /* Previous length range */
        const size_t prev_start = offset - (size_t)(len - 2);
        const size_t prev_end   = offset + (size_t)(len - 2);

        for (size_t i = prev_start; i <= prev_end; ++i) {
            seq_t v1 = cur1[i];
            seq_t v2 = cur2[i];
            seq_t v3 = cur3[i];

            if (v1 == 0 && v2 == 0 && v3 == 0)
                continue;

            /* Append letter 1 – difference unchanged */
            tmp1[i] += v1 + v2 + v3;

            /* Append letter 2 */
            tmp2[i + 1] += v1;   /* 1→2: d+1 */
            tmp2[i]     += v2;   /* 2→2: no change */
            tmp2[i]     += v3;   /* 3→2: no change */

            /* Append letter 3 */
            tmp3[i]     += v1;   /* 1→3: no change */
            tmp3[i - 1] += v2;   /* 2→3: d-1 */
            tmp3[i]     += v3;   /* 3→3: no change */
        }

        /* Prepare for next iteration: swap current and temporary arrays */
        seq_t *swap;
        swap = cur1; cur1 = tmp1; tmp1 = swap;
        swap = cur2; cur2 = tmp2; tmp2 = swap;
        swap = cur3; cur3 = tmp3; tmp3 = swap;
    }

    /* Final answer: all words of length n with difference zero */
    seq_t result = cur1[offset] + cur2[offset] + cur3[offset];

    /* Release allocated memory */
    free(cur1);
    free(cur2);
    free(cur3);
    free(tmp1);
    free(tmp2);
    free(tmp3);

    return result;
}
