/*
 * A017839  Expansion of 1/(1-x^5-x^6-x^7-x^8).
 * https://oeis.org/A017839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-6) + a(n-7) + a(n-8) for n > 7, a(0) = 1,
 *            a(1) = a(2) = a(3) = a(4) = 0, a(5) = a(6) = a(7) = a(8) = 1
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 110, u64 n <= 212,
 *            u128 n <= 415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *CACHE = NULL;          /* array of computed values             */
static idx_t MAX_COMPUTED = (idx_t)-1;/* highest n that has been computed      */
static size_t CACHE_SIZE = 0;        /* size (in elements) of CACHE memory    */

/* Initialise CACHE with the first 9 terms.  */
static void init_cache(void)
{
    if (CACHE != NULL)
        return;

    /* allocate space for indices 0 … 8 */
    CACHE_SIZE = 9;
    CACHE = (seq_t *)malloc(CACHE_SIZE * sizeof(seq_t));
    if (CACHE == NULL)
        abort();                       /* out‑of‑memory */

    /* set initial terms explicitly; the rest default to 0 */
    CACHE[0] = 1;
    /* indices 1..4 are already 0 */
    CACHE[5] = 1;
    CACHE[6] = 1;
    CACHE[7] = 1;
    CACHE[8] = 1;

    MAX_COMPUTED = 8;
}

/* Compute a(n) using the recurrence and memoise values. */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= MAX_COMPUTED)
        return CACHE[n];

    /* Expand CACHE so that it can hold indices up to n. */
    size_t new_size = (size_t)n + 1;
    if (new_size > CACHE_SIZE) {
        seq_t *tmp = (seq_t *)realloc(CACHE, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();                   /* out‑of‑memory */
        CACHE = tmp;
        CACHE_SIZE = new_size;
    }

    /* Compute all values from (MAX_COMPUTED + 1) up to n. */
    for (idx_t i = MAX_COMPUTED + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 5) sum += CACHE[i - 5];
        if (i >= 6) sum += CACHE[i - 6];
        if (i >= 7) sum += CACHE[i - 7];
        if (i >= 8) sum += CACHE[i - 8];
        CACHE[i] = sum;
    }

    MAX_COMPUTED = n;
    return CACHE[n];
}
