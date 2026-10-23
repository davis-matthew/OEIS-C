/*
 * A043619  Numbers whose base-7 representation has exactly 4 runs.
 * https://oeis.org/A043619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑7 representation has exactly 4 runs. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b);
}

/* dynamic array to hold generated values */
static seq_t *num_arr      = NULL;
static size_t num_size     = 0;
static size_t num_cap      = 0;
static size_t max_len_gen  = 3;    /* last length that was generated */
static bool   needs_sort   = false;

/* grow the array if needed */
static void ensure_cap(size_t needed)
{
    if (needed <= num_cap)
        return;
    size_t new_cap = num_cap ? num_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    if (num_arr) {
        memcpy(new_arr, num_arr, num_size * sizeof(seq_t));
        free(num_arr);
    }
    num_arr = new_arr;
    num_cap = new_cap;
}

/* add one new value to the array */
static void add_value(seq_t val)
{
    ensure_cap(num_size + 1);
    num_arr[num_size++] = val;
    needs_sort = true;
}

/* generate all numbers of a given base‑7 length that have
   exactly four runs */
static void generate_length(size_t len)
{
    /* compositions of len into 4 positive integers */
    for (size_t c1 = 1; c1 <= len - 3; ++c1)
        for (size_t c2 = 1; c2 <= len - c1 - 2; ++c2)
            for (size_t c3 = 1; c3 <= len - c1 - c2 - 1; ++c3) {
                size_t c4 = len - c1 - c2 - c3;
                for (int d1 = 1; d1 <= 6; ++d1)            /* first digit cannot be 0 */
                    for (int d2 = 0; d2 <= 6; ++d2) if (d2 != d1)
                        for (int d3 = 0; d3 <= 6; ++d3) if (d3 != d2)
                            for (int d4 = 0; d4 <= 6; ++d4) if (d4 != d3) {
                                /* build the base-7 number */
                                seq_t val = 0;
                                for (size_t i = 0; i < c1; ++i)
                                    val = val * 7 + d1;
                                for (size_t i = 0; i < c2; ++i)
                                    val = val * 7 + d2;
                                for (size_t i = 0; i < c3; ++i)
                                    val = val * 7 + d3;
                                for (size_t i = 0; i < c4; ++i)
                                    val = val * 7 + d4;
                                add_value(val);
                            }
            }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined before offset, return 0 sensibly */
        return 0;

    /* generate until we have at least n terms */
    while (num_size < (size_t)n) {
        ++max_len_gen;
        generate_length(max_len_gen);
    }

    /* sort the array if new terms were added */
    if (needs_sort) {
        qsort(num_arr, num_size, sizeof(seq_t), cmp_seq_t);
        needs_sort = false;
    }

    return num_arr[n - 1];
}
