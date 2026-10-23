/*
 * A013948  Positions of 2's in Kolakoski sequence (A000002).
 * https://oeis.org/A013948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute positions of 2’s in the Kolakoski sequence (A013948).
   The algorithm generates the Kolakoski sequence on the fly,
   storing the 1‑based indices of the value 2.
   It keeps static state so that calls in any order are supported. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *kola = NULL;      /* Kolakoski values (1 or 2) */
static size_t kola_len = 0, kola_cap = 0;

static size_t run_idx = 0;     /* index in kola pointing to current run length */
static seq_t cur_val = 1;      /* value to output next (1 or 2) */
static size_t rem = 0;         /* remaining elements in current run */

static seq_t *pos = NULL;      /* 1‑based positions of the value 2 */
static size_t pos_len = 0, pos_cap = 0;

/* Ensure pos array can hold at least 'need' entries */
static void ensure_pos_cap(size_t need)
{
    if (pos_cap < need) {
        size_t new_cap = pos_cap ? pos_cap * 2 : 128;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pos, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pos = tmp;
        pos_cap = new_cap;
    }
}

/* Ensure kola array can hold at least 'need' entries */
static void ensure_kola_cap(size_t need)
{
    if (kola_cap < need) {
        size_t new_cap = kola_cap ? kola_cap * 2 : 128;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(kola, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        kola = tmp;
        kola_cap = new_cap;
    }
}

/* Generate the Kolakoski sequence until at least 'target' positions of 2 are recorded */
static void generate_until(size_t target)
{
    /* First call: initialise the sequence and state */
    if (kola_len == 0) {
        ensure_kola_cap(2);
        kola[0] = 1;
        kola[1] = 2;
        kola_len = 2;

        /* Record the first 2 at position 2 */
        ensure_pos_cap(1);
        pos[pos_len++] = (seq_t)2;

        /* Set up state for generating the rest.
           We have already output the first 2 at index 1.
           We will output the next 2 (index 2) next. */
        run_idx = 1;                 /* run length for the current run is kola[1] = 2 */
        cur_val = 2;                 /* next run is value 2 */
        rem = (size_t)kola[run_idx] - 1;  /* one 2 has already been output */
    }

    /* Extend until we have enough 2's */
    while (pos_len < target) {
        ensure_kola_cap(kola_len + 1);
        kola[kola_len] = cur_val;
        size_t index = kola_len;          /* 0‑based index of the new element */

        if (cur_val == 2) {
            ensure_pos_cap(pos_len + 1);
            pos[pos_len++] = (seq_t)(index + 1);  /* positions are 1‑based */
        }

        kola_len++;

        /* Update run counter: consume one element of the current run */
        rem--;
        if (rem == 0) {
            run_idx++;
            cur_val = 3 - cur_val;                 /* toggle between 1 and 2 */
            rem = (size_t)kola[run_idx];
        }
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    generate_until((size_t)n);
    return pos[n - 1];
}
