/*
 * A032942  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^(m-i) has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑7 representation has 0 on
 *            all positions with odd index when counted from the
 *            most‑significant digit
 * Range:     u8 n <= 19, u16 n <= 536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   z_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/z_of_idx.h"

seq_t a(idx_t n)
{
    /* static cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t used  = 0;

    /* state of the two interleaving sequences */
    static bool initialized = false;
    static idx_t idxA = 1, idxB = 1;
    static seq_t valA = 0, valB = 0;

    if (n == 0) return 0;        /* placeholder – n ≥ 1 in usage */

    if (!initialized) {
        valA = z_of_idx(idxA);          /* first element of A */
        valB = (seq_t)7 * z_of_idx(idxB);/* first element of 7·A */
        initialized = true;
    }

    while (used < n) {
        if (used >= cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            /* build environment guarantees allocation succeeds */
            cache = tmp;
            cap   = newcap;
        }

        /* choose the smaller next value from the two sequences */
        if (valA < valB) {
            cache[used++] = valA;
            ++idxA;
            valA = z_of_idx(idxA);
        } else {
            cache[used++] = valB;
            ++idxB;
            valB = (seq_t)7 * z_of_idx(idxB);
        }
    }

    return cache[n - 1];
}
