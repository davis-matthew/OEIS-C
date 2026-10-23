/*
 * A043663  Numbers whose base-14 representation has exactly 2 runs.
 * https://oeis.org/A043663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all base‑14 numbers whose representation has
   exactly two runs (two maximal blocks of equal digits). */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static int seq_cmp(const void *p1, const void *p2)
{
    seq_t x = *(const seq_t*)p1;
    seq_t y = *(const seq_t*)p2;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Find the minimal base‑14 length L that yields at least n two‑run numbers. */
    idx_t cumulative = 0;
    idx_t L = 1;
    while (cumulative < n) {
        ++L;
        cumulative += (idx_t)169 * (L - 1);   /* 13 choices for a (non‑zero),
                                                13 for b (≠a), (L-1) split positions */
    }
    /* cumulative now is the total count for lengths up to L. */
    size_t cap   = (size_t)cumulative;       /* capacity for array of seq_t */
    seq_t *arr   = alloc_seq_array(cap);
    if (!arr) return 0;                     /* allocation failure */

    size_t sz = 0;
    for (idx_t len = 2; len <= L; ++len) {
        for (idx_t a_digit = 1; a_digit <= 13; ++a_digit) {
            for (idx_t b_digit = 0; b_digit <= 13; ++b_digit) {
                if (b_digit == a_digit) continue;
                for (idx_t split = 1; split <= len - 1; ++split) {
                    seq_t v = 0;
                    for (idx_t i = 0; i < split; ++i)
                        v = v * (seq_t)14 + (seq_t)a_digit;
                    for (idx_t i = split; i < len; ++i)
                        v = v * (seq_t)14 + (seq_t)b_digit;
                    arr[sz++] = v;
                }
            }
        }
    }

    /* Sort the generated numbers to obtain them in increasing order. */
    qsort(arr, sz, sizeof(seq_t), seq_cmp);

    seq_t result = arr[n - 1];  /* n is 1‑based */
    free_seq_array(arr);
    return result;
}
