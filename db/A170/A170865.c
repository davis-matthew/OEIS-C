/*
 * A170865  a(0) = 1, a(1) = 1; a(2^i + j) = a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1;
 * for n≥2: let p = 2^⌊log₂(n)⌋ and j = n − p;
 * then a(n) = a(j) + 3·a(j+1) */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    static seq_t *tab = NULL;
    static idx_t sz = 0;               /* number of elements allocated */

    /* extend cache if needed */
    if ((size_t)n >= sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_tab = (seq_t*)realloc(tab, new_sz * sizeof(seq_t));
        if (!new_tab) abort();          /* allocation failure */
        tab = new_tab;

        for (idx_t i = sz; i <= n; ++i) {
            if (i == 0 || i == 1) {
                tab[i] = (seq_t)1;
            } else {
                idx_t p = ((idx_t)1) << floor_log2_uint64(i);
                idx_t j = i - p;
                tab[i] = tab[j] + ((seq_t)3 * tab[j + 1]);
            }
        }
        sz = new_sz;
    }
    return tab[n];
}
