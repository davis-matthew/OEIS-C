/*
 * A192163  Monotonic ordering of nonnegative differences 4^i-6^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search: list all non‑negative values of 4^i−6^j for 0≤i≤40 and j≥0,
   sort them ascending, remove duplicates, and return the n‑th term
   (a(1)=0). */
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the generated sequence */
static seq_t seq_values[5000];
static size_t seq_len = 0;
static bool seq_initialized = false;

/* comparator for qsort (works for integer or big‑integer seq_t instances) */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* initialise the sequence once */
static void init_sequence(void)
{
    if (seq_initialized)
        return;

    seq_len = 0;
    seq_t val_i = 1;          /* 4^0 */
    idx_t i = 0;

    while (i <= 40) {
        seq_t val_j = 1;      /* 6^0 */
        while (true) {
            if (val_j > val_i) /* stop when difference would become negative */
                break;

            if (seq_len >= 5000) /* safety guard */
                return;

            seq_values[seq_len++] = val_i - val_j;

            /* advance to next 6^j, detect overflow by wrap‑around */
            seq_t nxt_val_j = val_j * 6;
            if (nxt_val_j < val_j)
                break;
            val_j = nxt_val_j;
        }

        /* advance to next 4^i, detect overflow by wrap‑around */
        seq_t nxt_val_i = val_i * 4;
        if (nxt_val_i < val_i)
            break;
        val_i = nxt_val_i;
        ++i;
    }

    /* sort ascending */
    qsort(seq_values, seq_len, sizeof(seq_t), seq_compare);

    /* remove duplicates */
    size_t uniq = 0;
    for (size_t k = 0; k < seq_len; ++k) {
        if (uniq == 0 || seq_values[k] != seq_values[uniq - 1])
            seq_values[uniq++] = seq_values[k];
    }
    seq_len = uniq;

    seq_initialized = true;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (!seq_initialized)
        init_sequence();

    if (n == 0)        /* offset is 1; undefined for n<1 */
        return (seq_t)0;

    idx_t index = n - 1;
    if (index >= (idx_t)seq_len) /* out of range */
        return (seq_t)0;

    return seq_values[index];
}
