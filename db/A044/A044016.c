/*
 * A044016  Numbers k such that 2 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k (n≥1) such that * the
 *            decimal representation of k contains the digits 2 and 6 *
 *            adjacent in any order, while the representation of k+1 contains
 *            * neither 2 nor 6 adjacent
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has26_adjacent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has26_adjacent.h"

/* Return the n‑th term of sequence A044016 (offset 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms      */
    static size_t size = 0;       /* number of terms stored    */
    static size_t cap  = 0;       /* allocated capacity        */
    static idx_t  last_checked = 1;   /* next integer to test      */

    if (n == 0)                  /* offset 1 – caller should use n≥1 */
        return 0;

    /* Grow cache if needed. */
    if (n > size) {
        if (cap < n) {
            size_t new_cap = (n > 2) ? n * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();          /* out‑of‑memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* Generate terms until we have n of them. */
        while (size < n) {
            seq_t k = (seq_t)last_checked;
            if (has26_adjacent(k) && !has26_adjacent(k + 1))
                cache[size++] = k;
            last_checked++;
        }
    }

    return cache[n - 1];
}
