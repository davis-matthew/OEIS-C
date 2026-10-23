/*
 * A033000  If d,e are consecutive digits of n in base 12, then |d-e|>=5.
 * https://oeis.org/A033000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   base12_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033000: numbers in base 12 whose consecutive digits differ by ≥ 5.
   The function generates the sequence by testing successive integers, keeping
   a static cache for efficiency. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/base12_ok.h"

static seq_t *cache = NULL;      /* cache of sequence terms (a(1), a(2),...)   */
static size_t cap   = 0;          /* allocated capacity of the cache          */
static size_t len   = 0;          /* number of terms stored in the cache      */

/* Ensure the cache can hold at least "needed" elements.  */
static void
ensure_capacity(size_t needed) {
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real program you might call abort or handle the error
               however here we just return */ 
            return;
        }
        cache = tmp;
        cap = new_cap;
    }
}

/* Return the n‑th term of the sequence.  The cache allows O(1) subsequent
   accesses for already computed terms. */
seq_t
a(idx_t n) {
    if (n == 0)
        return 0;              /* out of range; defined behaviour is irrelevant */

    /* Generate terms until we have at least n terms stored. */
    if (n > len) {
        seq_t cur = (len == 0) ? 1 : cache[len - 1] + 1;
        while (len < (size_t)n) {
            if (base12_ok(cur)) {
                ensure_capacity(len + 1);
                cache[len++] = cur;
            }
            cur++;
        }
    }
    return cache[(size_t)(n - 1)];
}
