/*
 * A080240  Define two sequences by A_n = mex{A_i,B_i : 0 <= i < n} for n >=
 *          0, B_0=0, B_1=1 and for n >= 2, B_n = 2B_{n-1}+(-1)^{A_n}.
 *          Sequence gives A_n.
 * https://oeis.org/A080240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{a(i), b(i) : 0 <= i < n} with b(0) = 0, b(1) = 1, and
 *            for i >= 2, b(i) = 2 * b(i-1) + (-1)^{a(i)}
 * Range:     u8 n <= 0, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

/* Static state for memoization */
static idx_t          computed   = 0;     /* number of A values computed so far */
static seq_t *        a_vals     = NULL;  /* array of computed A values     */
static size_t         a_cap      = 0;     /* capacity of a_vals array       */

static size_t         present_cap = 0;     /* capacity of the inclusion array */
static bool *         present     = NULL;  /* present[x] == true if x appears in {A_i,B_i} */
static seq_t          current_mex = 0;     /* current mex (smallest non‑present value) */

static seq_t          B_prev = 0;         /* current B_ value */

/* Ensure the inclusion array can accommodate indices up to `needed`. */
static void ensure_present_cap(size_t needed)
{
    if (needed < present_cap)
        return;
    size_t new_cap = present_cap ? present_cap << 1 : 256;
    while (new_cap <= needed)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(present, new_cap * sizeof(bool));
    if (!tmp)
        abort();                /* allocation failure */
    /* Zero the newly allocated part */
    if (present_cap < new_cap)
        memset(tmp + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
    present = tmp;
    present_cap = new_cap;
}

/* Ensure the array of A values can accommodate index `need`. */
static void ensure_a_cap(size_t need)
{
    if (need < a_cap)
        return;
    size_t new_cap = a_cap ? a_cap << 1 : 256;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_vals = tmp;
    a_cap = new_cap;
}

/* Mark a value in the inclusion set, updating the mex if necessary. */
static void mark_value(seq_t val)
{
    if (val >= (seq_t)present_cap)
        return;

    size_t idx = (size_t)val;
    if (!present[idx]) {
        present[idx] = true;
        /* If the newly marked value is the current mex, advance it. */
        if ((seq_t)idx == current_mex) {
            while ((seq_t)present_cap > current_mex && present[(size_t)current_mex])
                ++current_mex;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* If already known, return the stored value. */
    if (n < (idx_t)computed)
        return a_vals[n];

    /* Initialise state on first call. */
    if (!present) {
        ensure_present_cap(256);
        /* Present array already zeroed by ensure_present_cap. */
        current_mex = 0;
        B_prev = 0;          /* B_0 */
    }
    if (!a_vals) {
        ensure_a_cap(256);
    }

    /* Compute values up to index n. */
    while ((idx_t)computed <= n) {
        /* Get current mex as the new A value. */
        seq_t A_i = current_mex;
        ensure_a_cap(computed);
        a_vals[computed] = A_i;
        mark_value(A_i);

        /* Handle B values */
        if (computed == 0) {
            /* B_0 = 0 is already stored in B_prev. Just mark it. */
            mark_value((seq_t)0);
            /* B_prev remains 0 for the next step. */
        } else if (computed == 1) {
            /* B_1 is explicitly 1. */
            mark_value((seq_t)1);
            B_prev = (seq_t)1;
        } else {
            /* Recurrence: B_i = 2*B_{i-1} + (-1)^{A_i}. */
            seq_t newB = B_prev * (seq_t)2;
            if ((A_i & (seq_t)1) == 0)
                newB += (seq_t)1;     /* (-1)^{even} = +1 */
            else
                newB -= (seq_t)1;     /* (-1)^{odd}  = -1 */
            if (newB < (seq_t)present_cap)
                mark_value(newB);
            B_prev = newB;
        }

        ++computed;
        /* Ensure we can find the next mex if needed. */
        if ((seq_t)present_cap <= current_mex)
            ensure_present_cap((size_t)current_mex + 128);
    }

    return a_vals[n];
}
