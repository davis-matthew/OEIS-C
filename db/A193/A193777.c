/*
 * A193777  Number of signed permutations of size 2n invariant under D and
 *          D'bar and avoiding (-2, 1) and (2, -1).
 * https://oeis.org/A193777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + n*a(n-2) - * Sum_{j=1..n-3}
 *            j*a(j)*A000085(n-j-2); * Initial values: a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---- cached values ----------------------------------------------------- */
static seq_t *seq_a = NULL;   /* a(index) values */
static size_t a_cap = 0;      /* capacity of seq_a */
static idx_t a_len = 0;       /* number of values stored */

static seq_t *seq_invol = NULL;   /* A000085(i) values */
static size_t invol_cap = 0;      /* capacity of seq_invol */
static idx_t invol_len = 0;       /* number of values stored */

/* ---- utility: grow array ------------------------------------------------ */
static void ensure_a_capacity(idx_t idx)
{
    if ((idx + 1) > (idx_t)a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 16;
        while ((idx_t)new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_a, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_a = tmp;
        a_cap = new_cap;
    }
}

static void ensure_invol_capacity(idx_t idx)
{
    if ((idx + 1) > (idx_t)invol_cap) {
        size_t new_cap = invol_cap ? invol_cap << 1 : 16;
        while ((idx_t)new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_invol, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_invol = tmp;
        invol_cap = new_cap;
    }
}

/* ---- compute involutions up to given index ----------------------------- */
static void compute_invol_upto(idx_t up_to)
{
    if (invol_len == 0) {
        ensure_invol_capacity(1);
        seq_invol[0] = (seq_t)1;
        seq_invol[1] = (seq_t)1;
        invol_len = 2;
    }
    for (idx_t k = invol_len; k <= up_to; ++k) {
        ensure_invol_capacity(k);
        if (k == 0) seq_invol[0] = (seq_t)1;
        else if (k == 1) seq_invol[1] = (seq_t)1;
        else {
            seq_t term = seq_invol[k - 2];
            term *= (seq_t)(k - 1);
            seq_invol[k] = seq_invol[k - 1] + term;
        }
    }
    invol_len = up_to + 1;
}

/* ---- compute a(n) values up to n --------------------------------------- */
static void compute_a_upto(idx_t n)
{
    if (seq_a == NULL) {
        ensure_a_capacity(n);
        for (idx_t i = 0; i <= n; ++i) seq_a[i] = 0;
        a_len = 0;
    }

    for (idx_t i = a_len; i <= n; ++i) {
        ensure_a_capacity(i);
        if (i == 0) {
            seq_a[0] = (seq_t)1;
        } else if (i == 1) {
            seq_a[1] = (seq_t)2;
        } else {
            if (i >= 3) compute_invol_upto(i - 3);

            seq_t sum = (seq_t)0;
            if (i >= 3) {
                for (idx_t j = 1; j <= i - 3; ++j) {
                    idx_t k = i - j - 2;          /* n-j-2 */
                    seq_t term = seq_a[j];
                    term *= (seq_t)j;
                    term *= seq_invol[k];
                    sum += term;
                }
            }

            seq_t term = seq_a[i - 1];
            term *= (seq_t)2;
            term += seq_a[i - 2] * (seq_t)i;
            term -= sum;
            seq_a[i] = term;
        }
    }
    a_len = n + 1;
}

/* ---- public entry point ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n >= a_len) {
        compute_a_upto(n);
    }
    return seq_a[n];
}
