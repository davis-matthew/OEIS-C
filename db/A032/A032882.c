/*
 * A032882  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of positive integers whose base‑3 digits
 * satisfy d0 < d1 > d2 < d3 > d4 < … .
 * The numbers are produced in increasing length, which
 * coincides with increasing numeric value, and for each fixed
 * length they are generated in lexicographic order of the digits
 * (hence ascending order within that length). */

/* Standard library for memory allocation and abort on failure. */
#include <stdlib.h>

/* Helper that appends a value to a dynamic array. */
static void push_seq(seq_t **seqp, size_t *lenp, size_t *capp, seq_t val)
{
    if (*lenp == *capp) {
        size_t new_cap = (*capp == 0) ? 16 : (*capp << 1);
        seq_t *tmp = (seq_t *)realloc(*seqp, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        *seqp = tmp;
        *capp = new_cap;
    }
    (*seqp)[(*lenp)++] = val;
}

/* Recursive generator.  curIdx is the current (decreasing) index,
 * prevDigit is the digit at position curIdx+1, and val is the
 * value already formed for positions > curIdx. */
static void dfs_len(int curIdx, int prevDigit, seq_t val, int L,
                    seq_t **seqp, size_t *lenp, size_t *capp,
                    const seq_t *pow3)
{
    if (curIdx == 0) {                       /* the least significant digit */
        push_seq(seqp, lenp, capp, val);
        return;
    }
    int nextIdx = curIdx - 1;
    int need_greater = (nextIdx & 1);        /* odd index => >, even => < */
    for (int d = 0; d <= 2; ++d) {
        if (need_greater) {
            if (d <= prevDigit) continue;   /* must be strictly greater */
        } else {
            if (d >= prevDigit) continue;   /* must be strictly smaller */
        }
        seq_t newVal = val + ((seq_t)d) * pow3[nextIdx];
        dfs_len(nextIdx, d, newVal, L, seqp, lenp, capp, pow3);
    }
}

/* Generate all length‑L numbers and append them to the global array. */
static void generate_length(int L, seq_t **seqp, size_t *lenp,
                            size_t *capp, const seq_t *pow3)
{
    if (L == 1) {
        push_seq(seqp, lenp, capp, 1);
        push_seq(seqp, lenp, capp, 2);
        return;
    }
    for (int lead = 1; lead <= 2; ++lead) {
        seq_t val = ((seq_t)lead) * pow3[L - 1];
        dfs_len(L - 1, lead, val, L, seqp, lenp, capp, pow3);
    }
}

/* Public function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Global state for memoisation. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;           /* number of entries stored */
    static size_t seq_cap = 0;
    static int generated_len = 0;        /* longest length generated so far */

    static seq_t pow3[128];
    static int pow3_len = 0;             /* number of powers of 3 computed */

    /* Initialise powers of 3 on first call. */
    if (pow3_len == 0) {
        pow3[0] = 1;
        pow3_len = 1;
    }

    /* Generate new lengths until we have at least n terms. */
    while (seq_len < n) {
        ++generated_len;                 /* new length to generate */

        /* Ensure we have enough powers of 3. */
        while (pow3_len <= generated_len) {
            pow3[pow3_len] = pow3[pow3_len - 1] * ((seq_t)3);
            ++pow3_len;
        }

        /* Generate all numbers of the new length. */
        generate_length(generated_len, &seq, &seq_len, &seq_cap, pow3);
    }

    return seq[n - 1];                   /* zero‑based indexing */
}
