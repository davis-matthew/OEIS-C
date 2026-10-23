/*
 * A044339  Numbers k such that the string 0,7 occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A044339 – Numbers k such that the string 0,7 occurs in the base 10 representation of k but not of k-1.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return true iff x contains the adjacent digits 0 followed by 7. */
static inline bool contains07(seq_t x) {
    const seq_t TEN = 10;
    seq_t prev = TEN;               /* sentinel that is not 7 */
    while (x != 0) {
        seq_t cur = x % TEN;
        if (cur == 0 && prev == 7) /* pattern 0 7 in the normal order */
            return true;
        prev = cur;
        x /= TEN;
    }
    return false;
}

/* Return true iff k satisfies the definition of the sequence. */
static inline bool good(seq_t k) {
    const seq_t TEN = 10;
    seq_t t = k;

    /* Strip all trailing zeros. */
    while (t % TEN == 0)
        t /= TEN;

    /* Must end with 7 after stripping zeros. */
    if (t % TEN != 7)
        return false;

    /* Must have a preceding 0 immediately left of that 7. */
    if (t < TEN)                /* no preceding digit */
        return false;
    if ((t / TEN) % TEN != 0)   /* left neighbour not 0 */
        return false;

    /* Remove the trailing "07" block and check the remaining prefix. */
    seq_t prefix = t / 100;     /* drop the "07" */
    return !contains07(prefix);
}

/* a(n) – nth term of OEIS A044339 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* out of bounds – not used by the tests */

    static seq_t *cache = NULL;  /* stored sequence values */
    static size_t cap = 0;       /* allocated capacity of cache */
    static idx_t found = 0;      /* number of terms already computed */
    static seq_t next_k = 0;     /* next integer to test */

    /* Grow the cache if necessary. */
    if (cap < (size_t)n) {
        size_t new_cap = cap == 0 ? 16 : cap << 1;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* OOM – abort (should not happen in normal runs) */
            exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* Produce terms until we have at least n of them. */
    while (found < n) {
        ++next_k;
        if (good(next_k)) {
            ++found;
            cache[found - 1] = next_k;
        }
    }

    return cache[n - 1];
}
