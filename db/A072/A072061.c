/*
 * A072061  [t], 1+[t], [2t], 2+[2t], [3t], 3+[3t], ..., where t=tau =
 *          (1+sqrt(5))/2 and []=floor.
 * https://oeis.org/A072061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; For n>1: if n even a(n)=a(n-1)+n/2 else a(n)=smallest
 *            positive integer not yet appeared This implementation builds the
 *            sequence incrementally, keeping an array of seen integers and a
 *            cache of computed terms
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Ensure that the used array can hold index 'v'.  It grows
   geometrically when needed. */
static void ensure_used_capacity(unsigned char **used, size_t *cap, idx_t v)
{
    if ((idx_t)(*cap) <= v) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 1;
        while ((idx_t)new_cap <= v) new_cap <<= 1;
        unsigned char *new_arr = (unsigned char *)realloc(*used, new_cap);
        if (!new_arr) abort();
        memset(new_arr + *cap, 0, new_cap - *cap);
        *used = new_arr;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static state: sequence cache */
    static seq_t *seq = NULL;          /* array of computed terms, 1‑based in OEIS */
    static size_t seq_len = 0;         /* number of terms stored  (a(1) … a(seq_len)) */

    /* static state: used‑set */
    static unsigned char *used = NULL; /* byte per integer; 1 = used */
    static size_t used_cap = 0;        /* capacity of 'used' array */
    static idx_t   next_unused = 1;    /* smallest positive integer not yet used */

    /* --- Initialization on first call --- */
    if (seq_len == 0) {
        seq_len = 1;
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) abort();
        seq[0] = (seq_t)1;          /* a(1) = 1 */

        used_cap = 8;
        used = (unsigned char *)calloc(used_cap, 1);
        if (!used) abort();
        used[1] = 1;                /* value 1 is now used */
        next_unused = 2;            /* next candidate integer */
    }

    /* --- Extend the sequence up to index n --- */
    if ((size_t)n > seq_len) {
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t val;

            if ((i & 1) == 0) {                /* even n */
                idx_t half = i >> 1;            /* n/2 */
                val = seq[i - 2] + (seq_t)half; /* a(n-1) is at seq[i-2] (zero‑based) */
            } else {                           /* odd n */
                val = (seq_t)next_unused;
            }

            /* Track the value as used */
            idx_t idx_val = (idx_t)val;
            ensure_used_capacity(&used, &used_cap, idx_val);
            used[idx_val] = 1;

            /* Update the next unused integer */
            if ((i & 1) == 0) {                /* even case: only skip if already used */
                while (next_unused < (idx_t)used_cap && used[next_unused])
                    ++next_unused;
            } else {                           /* odd case: start search just above val */
                next_unused = idx_val + 1;
                while (next_unused < (idx_t)used_cap && used[next_unused])
                    ++next_unused;
            }

            /* Append the value to the cache */
            seq = (seq_t *)realloc(seq, (seq_len + 1) * sizeof(seq_t));
            if (!seq) abort();
            seq[seq_len] = val;
            ++seq_len;
        }
    }

    return seq[n - 1];   /* array is 0‑based, OEIS is 1‑based */
}
