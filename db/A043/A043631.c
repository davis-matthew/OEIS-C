/*
 * A043631  Numbers whose base-9 representation has exactly 2 runs.
 * https://oeis.org/A043631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 641, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A043631: numbers whose base‑9 representation has exactly two runs.
   The n‑th term (1‑based) is returned by generating all such numbers of increasing
   length until the list contains at least n entries, sorting that list, then
   picking the n‑th smallest.  The algorithm is O(n) and uses only integer
   arithmetic. */

#include <stdlib.h>

static int cmp_idx(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n==0 is undefined by the problem statement. */
    if (n == 0) return (seq_t)0;   /* safe fallback */

    static idx_t *arr      = NULL;   /* values of the sequence (stored as idx_t) */
    static size_t arr_size = 0;       /* current number of entries */
    static size_t arr_cap  = 0;       /* allocated capacity */
    static idx_t  d_done    = 1;      /* highest length processed */

    /* Find length d_needed such that total numbers up to that length >= n. */
    idx_t d_needed = 2;               /* smallest valid length is 2 */
    while ( (idx_t)32 * d_needed * (d_needed - 1) < n )
        ++d_needed;

    /* Generate new lengths if required. */
    if (d_needed > d_done) {
        for (idx_t d = d_done + 1; d <= d_needed; ++d) {
            for (int a = 1; a <= 8; ++a) {          /* first digit (run a) cannot be 0 */
                for (int b = 0; b <= 8; ++b) {      /* second run digit b (may be 0) */
                    if (b == a) continue;
                    for (idx_t k = 1; k <= d - 1; ++k) { /* length of first run */
                        idx_t val = 0;
                        for (idx_t i = 0; i < d; ++i) {
                            val = val * 9 + (i < k ? (idx_t)a : (idx_t)b);
                        }
                        /* Append to array, growing capacity as needed. */
                        if (arr_size == arr_cap) {
                            arr_cap = arr_cap ? arr_cap * 2 : 64;
                            arr = (idx_t *)realloc(arr, arr_cap * sizeof(idx_t));
                        }
                        arr[arr_size++] = val;
                    }
                }
            }
        }
        /* Sort the entire array after new entries are added. */
        qsort(arr, arr_size, sizeof(idx_t), cmp_idx);
        d_done = d_needed;
    }

    /* Return the n‑th element (1‑based index). */
    return (seq_t)arr[n - 1];
}
