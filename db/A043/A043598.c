/*
 * A043598  Numbers whose base-4 representation has exactly 7 runs.
 * https://oeis.org/A043598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑4 representation has exactly 7 runs.
   The function generates the terms on demand and stores them in a
   static cache.  The generation is a simple linear search, which
   is sufficiently fast for n up to at least 10 000. */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#endif

/* Count runs in the base‑4 representation of x.  For x = 0 the lone
   digit “0” counts as a single run. */
static inline int runs_base4(seq_t x)
{
    if (x == 0) return 1;
    int cnt = 0;
    seq_t prev = 4;                /* sentinel value outside {0,1,2,3} */
    while (x != 0) {
        seq_t d = x % 4;
        if (d != prev) {
            ++cnt;
            prev = d;
        }
        x /= 4;
    }
    return cnt;
}

/* ----------------------------------------------------------------------
   Cache for the generated terms
   ---------------------------------------------------------------------- */
#if defined(__cplusplus)

/* C++ implementation: use std::vector for automatic memory handling. */
static std::vector<seq_t> seq_vec;
static seq_t last_checked = 0;

#else

/* C implementation: allocate a dynamic array with malloc/realloc.
   This is safe because in a pure C build seq_t is a plain integer type. */
static seq_t *seq_arr   = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static seq_t last_checked = 0;

static void grow_seq_arr(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

#endif

/* ----------------------------------------------------------------------
   Public function
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* 0 is undefined for offset 1 */

#if defined(__cplusplus)

    /* Grow the vector until it contains at least n terms. */
    while (seq_vec.size() < (size_t)n) {
        ++last_checked;
        if (runs_base4(last_checked) == 7)
            seq_vec.push_back(last_checked);
    }
    return seq_vec[(size_t)n - 1];

#else

    /* Grow the C array until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (runs_base4(last_checked) == 7) {
            grow_seq_arr(seq_len + 1);
            seq_arr[seq_len++] = last_checked;
        }
    }
    return seq_arr[(size_t)n - 1];

#endif
}
