/*
 * A039264  Numbers whose base-12 representation has the same number of 6's
 *          and 11's.
 * https://oeis.org/A039264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑12 representation has the same number of 6's
 *            and 11's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: test if a number has an equal count of base‑12 digits 6 and 11 */
static bool has_balanced_digits(seq_t x) {
    if (x == (seq_t)0) return true;
    int count6 = 0, count11 = 0;
    seq_t tmp = x;
    const seq_t base = (seq_t)12;
    while (tmp > (seq_t)0) {
        seq_t d = tmp % base;
        if (d == (seq_t)6) ++count6;
        else if (d == (seq_t)11) ++count11;
        tmp = tmp / base;
    }
    return count6 == count11;
}

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;        /* number of terms stored */
    static idx_t capacity = 0;    /* allocated capacity of cache */
    static seq_t next = (seq_t)0; /* next candidate to test */

    if (n < (idx_t)1) return (seq_t)0; /* undefined for n<1 */

    /* generate terms until we have at least n elements */
    while (size < n) {
        seq_t cand = next;
        if (has_balanced_digits(cand)) {
            /* grow cache if needed */
            if (size >= capacity) {
                size_t new_cap = capacity ? (size_t)capacity * 2 : 64;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) /* allocation failure – return last value */
                    return cache[size - 1];
                cache = new_cache;
                capacity = (idx_t)new_cap;
            }
            cache[size++] = cand;
        }
        next = cand + (seq_t)1;
    }
    return cache[n - 1];
}
