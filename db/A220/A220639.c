/*
 * A220639  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly one king-move neighbor.
 * https://oeis.org/A220639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A220639
    Formula: a(n) = 8*a(n-1)+22*a(n-2)-20*a(n-3)-16*a(n-4)+10*a(n-5)-3*a(n-6)
    Initial values:
          a(0)=1, a(1)=3, a(2)=40, a(3)=369, a(4)=3755, a(5)=37320
    The function supports calls in any order and memoises all computed terms.
*/
#include <stdlib.h>

static seq_t *cache   = NULL;   /* array holding computed values   */
static size_t  cap    = 0;      /* current capacity of the cache  */
static size_t  computed = 0;    /* number of values stored (0…computed‑1) */

/* Ensure cache can hold indices 0 … n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap)   /* already big enough */
        return;

    size_t new_cap = cap ? cap * 2 : 32;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* fatal error – unable to allocate more memory */
        exit(1);
    }
    cache = tmp;
    cap   = new_cap;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    /* Expand storage if necessary */
    ensure_capacity(n);

    /* Initialise base cases only once */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)369;
        cache[4] = (seq_t)3755;
        cache[5] = (seq_t)37320;
        computed = 6;
    }

    /* Compute any missing terms up to n */
    while (computed <= n) {
        idx_t k = computed;
        seq_t val = 8 * cache[k - 1];
        val += 22 * cache[k - 2];
        val -= 20 * cache[k - 3];
        val -= 16 * cache[k - 4];
        val += 10 * cache[k - 5];
        val -= 3  * cache[k - 6];
        cache[computed++] = val;
    }

    return cache[n];
}
