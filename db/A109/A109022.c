/*
 * A109022  Integers with mutual residues of 2 or more.
 * https://oeis.org/A109022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimum integer m > a(n-1) such that for all i < n, m mod a(i) >= 2 */
#include <stdlib.h>
#include <stdbool.h>

/* cache for the sequence */
static seq_t *seq_arr = NULL;   /* array of previously computed terms */
static idx_t seq_len = 0;       /* number of terms stored in seq_arr */
static size_t seq_cap = 0;      /* allocated capacity of seq_arr */

/* ensure that the cache can hold at least `needed` terms */
static void ensure_capacity(idx_t needed)
{
    if (seq_cap < (size_t)needed) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        if (new_cap < (size_t)needed) {
            new_cap = needed;
        }
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)   /* n >= 1, 1‑based indexing */
{
    if (n == (idx_t)0) {
        return (seq_t)0;   /* undefined, but harmless */
    }

    /* initialise cache with first term if empty */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq_arr[0] = (seq_t)3;   /* a(1) = k+1 = 3 for k = 2 */
        seq_len = 1;
    }

    /* generate terms up to the requested index */
    if (seq_len < n) {
        ensure_capacity(n);
        for (idx_t i = seq_len; i < n; ++i) {
            seq_t candidate = seq_arr[i - 1] + (seq_t)1;
            for (;;) {
                bool ok = true;
                for (idx_t j = 0; j < i; ++j) {
                    if ((candidate % seq_arr[j]) < (seq_t)2) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    break; /* candidate satisfies all residues */
                }
                ++candidate;
            }
            seq_arr[i] = candidate;
        }
        seq_len = n;
    }

    return seq_arr[n - 1];
}
