/*
 * A214992  Power ceiling-floor sequence of (golden ratio)^4.
 * https://oeis.org/A214992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A214992
   Formula (recurrence):
       a(0) = 7
       a(1) = 47
       a(2) = 323
       a(n) = 6*a(n-1) + 6*a(n-2) - a(n-3)   for n ≥ 3
*/
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;           /* array of computed terms   */
static size_t cap = 0;                /* current capacity in terms */
static idx_t last_computed = (idx_t)-1;/* last index that is stored   */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cap) return;              /* already enough space */

    size_t new_cap = cap ? cap << 1 : 4;       /* start with 4      */
    while (new_cap <= (size_t)n) new_cap <<= 1;/* grow until enough */

    /* realloc is C++’s void* return; cast explicitly. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                        /* memory allocation failed */

    cache = tmp;
    cap = new_cap;
}

/* Initialise the first three values of the sequence. */
static void init_base(void)
{
    ensure_capacity(2);                       /* need indices 0 … 2 */

    cache[0] = 7;
    cache[1] = 47;
    cache[2] = 323;
    last_computed = 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 7;
    if (n == 1) return 47;
    if (n == 2) return 323;

    /* first call – set up the cache */
    if (last_computed == (idx_t)-1) init_base();

    ensure_capacity(n);

    if (n <= last_computed) return cache[n];

    /* compute all unseen terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i-1] + (seq_t)6 * cache[i-2] - cache[i-3];
    }

    last_computed = n;
    return cache[n];
}
