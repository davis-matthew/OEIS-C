/*
 * A022432  a(n) = c(n-1) + c(n-3) where c is the sequence of numbers not in
 *          a.
 * https://oeis.org/A022432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A022432
   Recurrence: a(n) = c(n‑1) + c(n‑3)  (n ≥ 3)
   where c is the sequence of natural numbers NOT appearing in a.
   Initial values: a(0)=1, a(1)=3, a(2)=4.
   The complement c is built incrementally while extending a. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- static state --- */
static seq_t *a_arr   = NULL;   /* array of a-values */
static size_t a_cap   = 0;
static size_t a_len   = 0;

static seq_t *c_arr   = NULL;   /* array of c-values (complement) */
static size_t c_cap   = 0;
static size_t c_len   = 0;

static bool *present = NULL;    /* present[x] == true ⇔ x ∈ a */
static size_t present_cap = 0;   /* current capacity of present[] */

static idx_t cur_val = 1;       /* next natural number to test for c */
static bool initialized = false;

/* --- helper: ensure present array can index up to needed --- */
static void ensure_present(idx_t needed)
{
    size_t need_index = (size_t)needed;          /* cast once for safety */
    if (needed < (idx_t)present_cap) return;

    size_t new_cap = present_cap ? present_cap << 1 : 16;
    while (new_cap <= need_index) new_cap <<= 1;

    bool *newp = (bool*)realloc(present, new_cap * sizeof(bool));
    if (newp == NULL) exit(EXIT_FAILURE);      /* allocation failure */
    /* zero the new entries */
    memset(newp + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
    present = newp;
    present_cap = new_cap;
}

/* --- helper: ensure a_arr has capacity at least needed --- */
static void ensure_a_cap(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *newp = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
    if (newp == NULL) exit(EXIT_FAILURE);
    a_arr = newp;
    a_cap = new_cap;
}

/* --- helper: ensure c_arr has capacity at least needed --- */
static void ensure_c_cap(size_t needed)
{
    if (needed <= c_cap) return;
    size_t new_cap = c_cap ? c_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *newp = (seq_t*)realloc(c_arr, new_cap * sizeof(seq_t));
    if (newp == NULL) exit(EXIT_FAILURE);
    c_arr = newp;
    c_cap = new_cap;
}

/* --- initialise static arrays and initial values --- */
static void init(void)
{
    /* allocate a_arr and insert the first three values */
    a_cap = 16;
    a_arr = (seq_t*)malloc(a_cap * sizeof(seq_t));
    a_len = 0;
    a_arr[a_len++] = (seq_t)1;
    a_arr[a_len++] = (seq_t)3;
    a_arr[a_len++] = (seq_t)4;

    /* no complement values yet */
    c_cap = 0;
    c_arr = NULL;
    c_len = 0;

    /* presence array */
    present = NULL;
    present_cap = 0;
    cur_val = 1;

    /* mark the initial a-values as present */
    ensure_present(4);                   /* at least up to 4 */
    present[1] = true;
    present[3] = true;
    present[4] = true;

    initialized = true;
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if ((size_t)n < a_len) return a_arr[(size_t)n];

    /* extend the sequence until a[n] is available */
    for (size_t idx = a_len; idx <= (size_t)n; ++idx) {
        /* generate complement values until we have at least idx‑1 elements */
        while (c_len <= idx - 1) {
            if (cur_val >= (idx_t)present_cap) ensure_present(cur_val);
            if (!present[(size_t)cur_val]) {
                ensure_c_cap(c_len + 1);
                c_arr[c_len++] = (seq_t)cur_val;
            }
            ++cur_val;
        }

        /* recurrence: a[idx] = c[idx-1] + c[idx-3] */
        seq_t val = c_arr[idx - 1] + c_arr[idx - 3];
        ensure_a_cap(idx + 1);
        a_arr[idx] = val;

        /* mark this value as present for future complement generation */
        if (val <= (seq_t)(present_cap - 1)) {
            ensure_present((idx_t)val);
            present[(size_t)val] = true;
        } else {
            /* value exceeds current present array size;
               extend it to accommodate this value */
            ensure_present((idx_t)val);
            present[(size_t)val] = true;
        }
    }

    return a_arr[(size_t)n];
}
