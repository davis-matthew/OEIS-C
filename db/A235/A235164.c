/*
 * A235164  Numbers whose digits, when the number is written in base n+1, are
 *          a permutation of 1...n, and such that for all k in {1,...,n} the
 *          first k digits (still in base n+1) form a number divisible by k.
 * https://oeis.org/A235164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is obtained by generating all numbers that satisfy the OEIS
 *            definition, inserting them in sorted order and returning the
 *            (n‑1)-th element (0‑based)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code works for both C11 and C++17.  In C++ we use std::vector,
   in C we use a resizable array with manual insertion. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

static idx_t next_base = 2;          /* next base to search, starts at 2 */

/* --------------------------------------------------------------------- */
/*  Storage of the generated sequence                                    */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec;

static void seq_insert(const seq_t &v)
{
    auto it = std::lower_bound(seq_vec.begin(), seq_vec.end(), v);
    seq_vec.insert(it, v);
}

static size_t seq_len_func(void) { return seq_vec.size(); }

static seq_t seq_at(idx_t i) { return seq_vec[(size_t)i]; }

#else
static seq_t *seq_arr = NULL;  /* dynamic array of found numbers */
static size_t seq_len   = 0;   /* current number of elements */
static size_t seq_cap   = 0;   /* allocated capacity */

static void seq_ensure(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap * 2 + 16;
        if (new_cap < need) new_cap = need;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

static void seq_insert(seq_t val)
{
    seq_ensure(seq_len + 1);
    /* insert sorted by shifting elements to the right */
    size_t i = seq_len;
    while (i > 0 && seq_arr[i-1] > val) { /* assumes operator> for seq_t */
        seq_arr[i] = seq_arr[i-1];
        --i;
    }
    seq_arr[i] = val;
    ++seq_len;
}

static size_t seq_len_func(void) { return seq_len; }

static seq_t seq_at(idx_t i) { return seq_arr[(size_t)i]; }
#endif

/* --------------------------------------------------------------------- */
/*  DFS generation for a given base b (b must be even)                    */
/* --------------------------------------------------------------------- */
static void dfs_generate(idx_t b, idx_t pos, seq_t val, bool *used)
{
    if (pos == b - 1) {          /* all digits placed */
        seq_insert(val);
        return;
    }

    idx_t next_len = pos + 1;
    idx_t next_d = next_len;      /* new prefix length is next_len */

    for (idx_t digit = 1; digit < b; ++digit) {
        if (used[digit]) continue;

        seq_t new_val = val * (seq_t)b + (seq_t)digit;

        /* check divisibility by new prefix length */
        if (new_val % (seq_t)next_d == 0) {
            used[digit] = true;
            dfs_generate(b, next_len, new_val, used);
            used[digit] = false;
        }
    }
}

static void generate_for_base(idx_t b)
{
    if (b % 2 == 1) return;          /* only even bases can produce solutions */

    bool *used = (bool *)calloc((size_t)(b + 1), sizeof(bool));
    if (!used) return;                /* out of memory: skip this base */

    dfs_generate(b, 0, (seq_t)0, used);

    free(used);
}

/* --------------------------------------------------------------------- */
/*  Public API: sequence a(idx_t n)                                    */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* produce terms until we have at least 'n' of them */
    while (seq_len_func() < n) {
        generate_for_base(next_base);
        ++next_base;
    }

    /* indices are 1‑based in OEIS; seq_… is 0‑based */
    return seq_at(n - 1);
}
