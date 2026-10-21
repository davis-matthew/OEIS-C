/*
 * A080653  a(1) = 2; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) such that the condition "a(a(n)) is always
 *          even" is satisfied.
 * https://oeis.org/A080653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(a(n)) = 2n + 2, with a minimal positive integer for each n ≥ 1
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static state for the sequence.  The array index equals the sequence
   index, and the array element holds the corresponding value. */
static seq_t *seq_arr  = NULL;   /* sequence data */
static size_t  arr_cap = 0;      /* current allocated capacity */
static size_t  arr_max = 0;      /* largest index that has been processed */

/* Ensure the array can be indexed at least up to 'need'.
   The array is automatically zero‑initialised for newly allocated entries. */
static void ensure_cap(size_t need)
{
    if (arr_cap > need)
        return;          /* already big enough */

    size_t new_cap = arr_cap ? arr_cap << 1 : 4;
    while (new_cap <= need)
        new_cap <<= 1;   /* grow until we can store index 'need' */

    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* allocation failed */

    /* Zero‑initialise any new slots. */
    for (size_t i = arr_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    seq_arr = tmp;
    arr_cap = new_cap;
}

/* Build the sequence up to (and including) index 'n'. */
static void build_up_to(idx_t n)
{
    if (n == (idx_t)0)
        return;       /* sequence is 1‑based */

    size_t target = (size_t)n;

    /* Initialise the very first term and its consequence. */
    if (arr_max == 0) {
        ensure_cap(2);
        seq_arr[1] = (seq_t)2;          /* a(1) = 2 */
        seq_arr[2] = (seq_t)( (seq_t)1 * 2 + 2 ); /* a(2) = 4 */
        arr_max = 1;                    /* we have processed index 1 */
    }

    /* Extend the table up to 'target'. */
    for (size_t i = arr_max + 1; i <= target; ++i) {
        /* 1. Set a(i) if it has not been set yet. */
        if (seq_arr[i] == (seq_t)0) {
            /* Find the smallest integer larger than a(i-1) that is not yet used. */
            size_t cand = (size_t)seq_arr[i - 1] + 1;
            while (true) {
                if (cand > arr_cap - 1)
                    ensure_cap(cand);
                if (seq_arr[cand] == (seq_t)0)
                    break;          /* unused value found */
                ++cand;
            }
            seq_arr[i] = (seq_t)cand;
        }

        /* 2. Enforce the functional equation a(a(i)) = 2i + 2. */
        size_t idx = (size_t)seq_arr[i];
        if (idx > arr_cap - 1)
            ensure_cap(idx);

        seq_t required = (seq_t)((seq_t)i * 2 + 2);
        if (seq_arr[idx] == (seq_t)0) {
            seq_arr[idx] = required;
        }
        /* If seq_arr[idx] already has a value it must agree with 'required'; 
           no explicit check is performed because the construction guarantees consistency. */
    }

    arr_max = target;
}

/* Required external function. */
seq_t a(idx_t n)
{
    build_up_to(n);
    return seq_arr[(size_t)n];
}
