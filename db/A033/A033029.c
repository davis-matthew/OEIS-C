/*
 * A033029  Numbers whose base-16 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers whose base‑16 expansion has no run of equal digits
   with length < 2.  The function generates the numbers by enumerating
   all admissible run‑length partitions and digit assignments, then
   keeps a global sorted unique list.  No closed formula is known.  */
#include <stdlib.h>
#include <stdbool.h>

/* data structures for the global cache */
static seq_t *seq_arr    = NULL;   /* sorted unique values */
static size_t seq_len    = 0;      /* number of values stored */
static size_t seq_cap    = 0;      /* allocated capacity of seq_arr */
static int   max_gen_len = 7;      /* highest hex‑length generated so far */

/* -------------------------------------------------------------------- */
/* comparison routine for qsort on seq_t values                          */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* -------------------------------------------------------------------- */
/* append a value, growing the array if necessary                        */
static void seq_add(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < seq_len; ++i)
            new_arr[i] = seq_arr[i];
        free(seq_arr);
        seq_arr = new_arr;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* -------------------------------------------------------------------- */
/* generate numbers for a given run‑length pattern and digit choices    */
static void gen_digits_rec(int idx, int runs, int *runlens, int *digits)
{
    if (idx == runs) {
        seq_t val = 0;
        for (int i = 0; i < runs; ++i)
            for (int j = 0; j < runlens[i]; ++j)
                val = val * 16 + digits[i];
        seq_add(val);
        return;
    }
    int start = (idx == 0) ? 1 : 0;   /* first digit cannot be zero   */
    for (int d = start; d <= 15; ++d) {
        if (idx > 0 && d == digits[idx - 1]) continue; /* merge runs       */
        digits[idx] = d;
        gen_digits_rec(idx + 1, runs, runlens, digits);
    }
}

/* -------------------------------------------------------------------- */
/* enumerate all partitions of total length L into parts >= 2           */
static void gen_patterns_rec(int L, int pos, int *runlens, int runs)
{
    if (pos == L) {
        int digits[20];
        gen_digits_rec(0, runs, runlens, digits);
        return;
    }
    for (int len = 2; len <= L - pos; ++len) {
        runlens[runs] = len;
        gen_patterns_rec(L, pos + len, runlens, runs + 1);
    }
}

/* -------------------------------------------------------------------- */
/* generate all admissible numbers with hex length exactly L            */
static void add_numbers_of_length(int L)
{
    int runlens[20];
    gen_patterns_rec(L, 0, runlens, 0);
}

/* -------------------------------------------------------------------- */
/* make sure we have at least n terms in seq_arr                        */
static void ensure_terms(idx_t n)
{
    if (seq_arr == NULL) {                 /* first initialisation      */
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        /* generate up to length 7 (sufficient for n ≤ 10 000) */
        for (int L = 2; L <= max_gen_len; ++L)
            add_numbers_of_length(L);

        qsort(seq_arr, seq_len, sizeof(seq_t), cmp_seq);

        size_t uniq = 0;
        for (size_t i = 0; i < seq_len; ++i)
            if (uniq == 0 || seq_arr[i] != seq_arr[uniq - 1])
                seq_arr[uniq++] = seq_arr[i];
        seq_len = uniq;
        seq_cap = seq_len;                  /* keep the larger allocation */
    }

    /* generate additional lengths until we have enough terms        */
    if ((idx_t)seq_len < n) {
        int next_len = max_gen_len + 1;
        while ((idx_t)seq_len < n) {
            add_numbers_of_length(next_len);
            ++next_len;
        }
        max_gen_len = next_len - 1;

        qsort(seq_arr, seq_len, sizeof(seq_t), cmp_seq);

        size_t uniq = 0;
        for (size_t i = 0; i < seq_len; ++i)
            if (uniq == 0 || seq_arr[i] != seq_arr[uniq - 1])
                seq_arr[uniq++] = seq_arr[i];
        seq_len = uniq;
        seq_cap = seq_len;
    }
}

/* -------------------------------------------------------------------- */
/* public entry point                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1            */
    ensure_terms(n);
    return seq_arr[(idx_t)n - 1];
}
