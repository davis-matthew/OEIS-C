/*
 * A044988  Numbers whose base-3 representation contains exactly four 0's and
 *          three 1's.
 * https://oeis.org/A044988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3790, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8686 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑3 representation contains exactly four 0's and three 1's.
   The array is built lazily; new numbers are generated in increasing base‑3
   length, guaranteeing the overall order.  */

#include <stdlib.h>
#include "../utils/mem.h"

/* Recursive helper:
   rem       – number of positions still to fill
   val       – value formed so far (including leading digit)
   zeros_l   – zeros needed in the remaining positions
   ones_l    – ones needed in the remaining positions   */
static void generate_rec(idx_t rem, seq_t val, int zeros_l, int ones_l,
                         seq_t **arr, size_t *len, size_t *cap)
{
    if (rem == 0) {
        if (zeros_l == 0 && ones_l == 0) {
            /* Append the number to the sequence array. */
            if (*len >= *cap) {
                *cap = (*cap == 0) ? 1024 : *cap << 1;
                *arr = mem_resize(seq_t, *arr, (*cap * sizeof(seq_t)) / sizeof(seq_t));
            }
            (*arr)[(*len)++] = val;
        }
        return;
    }

    /* Digit 0 – consumes a zero if still needed. */
    if (zeros_l > 0)
        generate_rec(rem - 1, val * 3 + 0, zeros_l - 1, ones_l, arr, len, cap);

    /* Digit 1 – consumes an one if still needed. */
    if (ones_l > 0)
        generate_rec(rem - 1, val * 3 + 1, zeros_l, ones_l - 1, arr, len, cap);

    /* Digit 2 – never changes counts. */
    generate_rec(rem - 1, val * 3 + 2, zeros_l, ones_l, arr, len, cap);
}

/* Generate all numbers of given length `len` whose base‑3 representation
   starts with `lead` (1 or 2) and contains exactly four 0's and three 1's. */
static void generate_len(idx_t len, int lead,
                         seq_t **seq, size_t *len_arr, size_t *cap)
{
    int zeros_needed = 4;
    int ones_needed  = 3;
    if (lead == 1)            /* the leading 1 already counts as one of the ones */
        ones_needed -= 1;

    generate_rec(len - 1, (seq_t)lead, zeros_needed, ones_needed,
                 seq, len_arr, cap);
}

/* Lazy sequence storage */
static seq_t *seq_array   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;
static idx_t next_len     = 7;         /* current base‑3 length to generate */

/* Ensure that the sequence contains at least `n` elements. */
static void ensure_n(idx_t n)
{
    while (seq_len < n) {
        generate_len(next_len, 1, &seq_array, &seq_len, &seq_cap);
        generate_len(next_len, 2, &seq_array, &seq_len, &seq_cap);
        next_len++;
    }
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)  return 0;          /* undefined, return 0 for safety */
    ensure_n(n);
    return seq_array[n - 1];
}
