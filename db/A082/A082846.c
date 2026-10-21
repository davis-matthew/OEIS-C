/*
 * A082846  a(1) = 1, a(2) = 2, a(3) = 3; then a(a(1)+a(2)+..+a(k)) = a(k) and
 *          fill the "holes" using the rule : a(m) = 1 if a(m-1) = 3, a(m) = 2
 *          if a(m-1) = 1, a(m) = 3 if a(m-1) = 2.
 * https://oeis.org/A082846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082846:  a(1)=1, a(2)=2, a(3)=3;
   For every k≥1, a(Σ_{i=1}^{k} a(i)) = a(k).
   All other indices are filled cyclically:
   if a(n‑1)=1 then a(n)=2; if a(n‑1)=2 then a(n)=3; if a(n‑1)=3 then a(n)=1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* One‑based arrays: A[1..], S[1..] */
    static seq_t *A = NULL;
    static seq_t *S = NULL;
    static idx_t len = 0;           /* number of computed terms */
    static idx_t cap = 0;           /* allocated capacity */
    static idx_t next_k_check = 1;  /* next k to be checked against the current index */

    /* Grow storage if needed */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 4 : (size_t)cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        A = (seq_t *)realloc(A, sizeof(seq_t) * (new_cap + 1));
        S = (seq_t *)realloc(S, sizeof(seq_t) * (new_cap + 1));
        cap = (idx_t)new_cap;
    }

    /* Initialise the first three terms on first call */
    if (len == 0) {
        A[1] = (seq_t)1;
        A[2] = (seq_t)2;
        A[3] = (seq_t)3;

        S[1] = A[1];
        S[2] = S[1] + A[2];
        S[3] = S[2] + A[3];

        len = 3;
        next_k_check = 1;
    }

    /* Compute terms up to n in increasing order */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* Advance next_k_check while the corresponding prefix sum is below i */
        seq_t target = (seq_t)i;
        while (next_k_check <= len && S[next_k_check] < target)
            ++next_k_check;

        seq_t val;
        if (next_k_check <= len && S[next_k_check] == target) {
            /* Forced term: match the previously defined cycle */
            val = A[next_k_check];
        } else {
            /* Gap: cycle from the previous value */
            seq_t prev = A[i - 1];
            if (prev == (seq_t)1)
                val = (seq_t)2;
            else if (prev == (seq_t)2)
                val = (seq_t)3;
            else /* prev == 3 */
                val = (seq_t)1;
        }

        A[i] = val;
        S[i] = S[i - 1] + val;
        ++len;
    }

    return A[n];
}
