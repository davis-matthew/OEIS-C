/*
 * A037977  Numbers whose maximal base 4 run length is 2.
 * https://oeis.org/A037977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose maximal base 4 run length is exactly 2. The
 *            function returns the n-th such number (with a(1) = 5)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Test whether x has a maximal run of exactly two identical base‑4 digits
   (i.e., contains at least one run of length 2 and no run of length >=3). */
static bool has_run_exact2(seq_t x)
{
    if (x == 0)          /* zero is not part of the sequence */
        return false;

    seq_t t = x;
    unsigned last = 0, run = 0;
    bool has2 = false;
    bool first = true;

    while (t > 0) {
        unsigned d = (unsigned)(t % 4);

        if (first) {
            last = d;
            run  = 1;
            first = false;
        } else {
            if (d == last) {
                run++;
                if (run > 2)        /* forbidden run of length 3 */
                    return false;
                if (run == 2)       /* found a run of length 2 */
                    has2 = true;
            } else {
                last = d;
                run  = 1;
            }
        }
        t /= 4;
    }
    return has2;
}

static seq_t *cache   = NULL;          /* array of found numbers     */
static size_t  cap    = 0;             /* current allocated capacity */
static idx_t   len    = 0;             /* number of elements stored  */
static seq_t   cand   = 1;             /* smallest candidate to test */

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n == 0, return 0 */
        return 0;

    /* Generate values up to index n if necessary. */
    while (len < n) {
        /* Find next candidate that satisfies the property */
        while (!has_run_exact2(cand))
            cand++;

        /* Ensure we have enough space to store it */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        cache[len++] = cand;   /* store the found value */
        cand++;                /* next number to test */
    }

    return cache[n - 1];        /* a(1) is stored at index 0 */
}
