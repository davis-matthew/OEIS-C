/*
 * A044653  Numbers n such that string 2,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Definition: a(n) is the nth integer (n ≥ 0) such that the base‑9 representation of
 * n contains the consecutive digits "23" while the base‑9 representation of n+1 does
 * not.  Offset 1 means a(1) = 21, the first such number.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the base‑9 representation of x contains the substring "23". */
static bool has23(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t d = x % 9;
        if (!first) {
            if (prev == 3 && d == 2) /* most‑significant digit d, least‑significant prev */
                return true;
        }
        first = false;
        prev = d;
        x /= 9;
    }
    return false;
}

/* Return the n-th term of sequence A044653 (offset 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t sz = 0;      /* number of terms stored */
    static idx_t cap = 0;     /* current capacity of cache */
    static seq_t cur = 0;     /* integer to test next */

    if (n == 0)              /* undefined, but return 0 for safety */
        return 0;

    /* Ensure cache has enough space to store the new term when needed. */
    while (sz < n) {
        if (sz == cap) {                 /* grow the cache */
            cap = (cap == 0) ? 64 : cap * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        /* Search for the next integer fulfilling the condition. */
        while (true) {
            if (has23(cur) && !has23(cur + 1)) {
                cache[sz++] = cur;
                cur++;                     /* start searching after this value */
                break;
            }
            cur++;
        }
    }
    return cache[n - 1];
}
