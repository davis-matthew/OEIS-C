/*
 * A137894  Limiting sequence when we start with positive integers (A000027)
 *          and at step n >= 1 add to the term at position n + a(n) the value
 *          n.
 * https://oeis.org/A137894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, for n≥1 add n to a(n+a(n)). Implements the definition
 *            given in OEIS A137894
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamically maintain the sequence up to the largest computed index. */
    static seq_t *seq = NULL;   /* array of values, 0‑based (seq[i] is a(i+1)) */
    static idx_t computed = 0;  /* largest index processed so far (1‑based) */
    static idx_t cap = 0;       /* current allocated size (number of elements) */

    if (n == 0) return 0; /* offset is 1; undefined for n==0 */

    /* Initialise on first call. */
    if (cap == 0) {
        cap = 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failed */
        seq[0] = (seq_t)1;           /* a(1)=1 */
    }

    /* Ensure we have computed all steps up to n. */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t ai = seq[i - 1];          /* current a(i) before update */
            idx_t target = i + (idx_t)ai;   /* 1‑based index to add to */

            /* Grow the array if necessary, initialising new positions to j+1. */
            if (target > cap) {
                idx_t new_cap = cap;
                while (new_cap < target) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failed */
                /* initialise new entries with their natural values (j+1). */
                for (idx_t j = cap; j < new_cap; ++j)
                    tmp[j] = (seq_t)(j + 1);
                seq = tmp;
                cap = new_cap;
            }

            seq[target - 1] += (seq_t)i; /* apply the update. */
        }
        computed = n;
    }

    return seq[n - 1];
}
