/*
 * A169632  The even terms in Recamán's sequence (A005132).
 * https://oeis.org/A169632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the even terms of Recamán's sequence (A169632).
 * The function generates Recamán's sequence a(0)=0, a(n)=a(n-1)-n if that
 * result is positive and not previously seen, otherwise a(n)=a(n-1)+n.
 * It keeps track of seen values in a dynamic boolean array and returns
 * the nth even term (n ≥ 1). */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Returns the nth even term of Recamán's sequence (1‑based index). */
seq_t a(idx_t n) {
    /* a(1) = 0 – the first term is even */
    if (n == 1) return (seq_t)0;

    seq_t cur = (seq_t)0;          /* current Recamán value */
    idx_t  step = 1;               /* n-th step in Recamán sequence (n≥1) */
    idx_t  even_cnt = 1;           /* count of even values seen so far */

    /* Dynamic boolean array of seen Recamán values.
     * seen[i] == true  ⇔  value i has been produced previously. */
    idx_t seen_size = 1;
    bool *seen = (bool*)malloc(sizeof(bool));
    if (!seen) return (seq_t)0;    /* allocation failed – return 0 */
    seen[0] = true;                /* 0 is seen */

    for (;;) {
        seq_t cand;
        /* Try to step backward; only if the result is positive and unseen. */
        if (cur > (seq_t)step) {
            cand = cur - (seq_t)step;
            bool used = false;
            if ((idx_t)cand < seen_size) used = seen[(idx_t)cand];
            if (!used) {
                cur = cand;
            } else {
                cur = cur + (seq_t)step;
            }
        } else {
            cur = cur + (seq_t)step;
        }

        /* Ensure the boolean array can index the new value. */
        idx_t cur_idx = (idx_t)cur;
        if (cur_idx >= seen_size) {
            idx_t new_size = cur_idx + 1;
            bool *tmp = (bool*)realloc(seen, new_size * sizeof(bool));
            if (!tmp) {
                free(seen);
                return (seq_t)0;   /* allocation failed */
            }
            /* Zero‑initialize the newly allocated portion. */
            memset(tmp + seen_size, 0, (new_size - seen_size) * sizeof(bool));
            seen = tmp;
            seen_size = new_size;
        }
        seen[cur_idx] = true;

        /* If this value is even, count it. */
        if ((cur & (seq_t)1) == 0) {
            even_cnt++;
            if (even_cnt == n) {
                free(seen);
                return cur;
            }
        }
        step++;
    }
}
