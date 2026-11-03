/*
 * A101594  Numbers with exactly two distinct decimal digits, neither of which
 *          is 0.
 * https://oeis.org/A101594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less, look
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 1424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9797 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101594: numbers with exactly two distinct non‑zero decimal digits */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}

/* Check whether x contains exactly two distinct digits from 1‑9 and no zero */
static inline bool has_two_nonzero_distinct(seq_t x)
{
    unsigned int mask = 0;   /* bit mask for digits 1‑9 */
    unsigned int cnt   = 0;   /* number of distinct digits seen */

    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d == 0)          /* zero is not allowed */
            return false;

        unsigned int bit = 1U << d;
        if ((mask & bit) == 0) {   /* new distinct digit */
            mask |= bit;
            ++cnt;
            if (cnt > 2)          /* more than two distinct digits */
                return false;
        }
        x /= 10;
    }
    return cnt == 2;           /* exactly two distinct digits */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state to cache already found terms */
    static seq_t *buf   = NULL;   /* array of terms found so far */
    static idx_t  sz    = 0;      /* number of stored terms */
    static idx_t  cap   = 0;      /* allocated capacity of buf        */
    static seq_t  cand  = 12;     /* next integer to test (first term) */

    /* Generate terms until we have at least n terms */
    while (sz < n) {
        if (has_two_nonzero_distinct(cand)) {
            /* Grow buffer if needed */
            if (sz == cap) {
                idx_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_buf = resize_array(buf, sz, new_cap);
                if (new_buf == NULL)     /* allocation failure: abort */
                    exit(1);
                buf = new_buf;
                cap = new_cap;
            }
            buf[sz++] = cand;          /* store this term */
        }
        cand += 1;                     /* test next integer */
    }

    return buf[n - 1];                  /* return the n‑th term (1‑based) */
}
