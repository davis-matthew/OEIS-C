/*
 * A081695  From P-positions in a certain game. Pile 2 tokens for select
 *          function (1+(-)^(y1+1))x1/2.
 * https://oeis.org/A081695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = mex{ a(i), b(i) : 0 <= i < n }; b(n) = a(n) + b(n-1) - (1
 *            + (-1)^b(n-1)) * a(n-1) / 2. Initial values: a(0) = b(0) = 0
 * Range:     u8 n <= 25, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Ensure that the array *arr has room for indices up to |idx|. */
static void ensure_seq_cap(idx_t idx, seq_t **arr, size_t *cap)
{
    if ((size_t)idx >= *cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 1;
        while ((size_t)idx >= new_cap) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Ensure that the boolean marker array can index the value |v|. */
static void ensure_used_cap(seq_t v, uint8_t **used, size_t *cap)
{
    size_t need = (size_t)v;
    if (need >= *cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 1;
        while (need >= new_cap) new_cap <<= 1;
        *used = (uint8_t*)realloc(*used, new_cap);
        /* Newly allocated bytes are zeroed. */
        memset(*used + *cap, 0, new_cap - *cap);
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static storage for the two sequences and supporting data. */
    static seq_t *a_seq = NULL;          /* a[n] = mex */
    static seq_t *b_seq = NULL;          /* b[n] = current sequence term */
    static size_t a_cap = 0, b_cap = 0;
    static uint8_t *used = NULL;         /* used[value] == 1 if value seen in a or b */
    static size_t used_cap = 0;
    static seq_t mex = 1;                /* minimal excluded, start after initial 0 */
    static idx_t max_n = (idx_t)-1;      /* highest index computed so far */

    /* initialise on first call */
    if (max_n == (idx_t)-1) {
        a_cap = b_cap = 1;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_seq = (seq_t*)malloc(b_cap * sizeof(seq_t));
        a_seq[0] = 0;
        b_seq[0] = 0;

        used_cap = 1;
        used = (uint8_t*)malloc(used_cap);
        memset(used, 0, used_cap);
        used[0] = 1;

        mex = 1;
        max_n = 0;
    }

    /* extend sequences up to n */
    if ((size_t)n > max_n) {
        /* ensure storage for new indices */
        ensure_seq_cap(n, &a_seq, &a_cap);
        ensure_seq_cap(n, &b_seq, &b_cap);

        /* compute entries from max_n+1 .. n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* 1. compute a[i] as current mex */
            seq_t ai = mex;

            /* mark ai as used */
            ensure_used_cap(ai, &used, &used_cap);
            used[(size_t)ai] = 1;

            /* advance mex to next unused value */
            while (true) {
                if ((size_t)mex >= used_cap) {
                    ensure_used_cap(mex, &used, &used_cap);
                }
                if (!used[(size_t)mex]) break;
                ++mex;
            }

            /* 2. compute b[i] via recurrence */
            seq_t bi = ai + b_seq[i-1];
            if ((b_seq[i-1] & 1) == 0) {      /* b[i-1] even */
                bi -= a_seq[i-1];
            }

            /* mark bi as used */
            ensure_used_cap(bi, &used, &used_cap);
            used[(size_t)bi] = 1;

            /* advance mex again if needed */
            while (true) {
                if ((size_t)mex >= used_cap) {
                    ensure_used_cap(mex, &used, &used_cap);
                }
                if (!used[(size_t)mex]) break;
                ++mex;
            }

            /* store results */
            a_seq[i] = ai;
            b_seq[i] = bi;
        }

        max_n = n;
    }

    return b_seq[n];
}
