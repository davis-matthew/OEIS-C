/*
 * A202637  x-values in the solution to x^2 - 7*y^2 = -3.
 * https://oeis.org/A202637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202637: x-values from x² – 7y² = –3
 *
 *  Recurrence:  a(n) = 16·a(n‑2) – a(n‑4)   for n ≥ 5
 *  Initial values:  a(1)=2,  a(2)=5,  a(3)=37,  a(4)=82.
 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of a(1)...a(k) */
static size_t capacity = 0;     /* current allocated length of cache   */
static size_t cached   = 0;     /* number of terms presently stored    */

/* enlarge cache if necessary */
static void ensure_capacity(size_t need)
{
    if (need <= capacity) return;

    size_t new_cap = capacity ? capacity : 1;
    while (new_cap < need) new_cap <<= 1;           /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                             /* allocation failure */

    cache = tmp;
    capacity = new_cap;
}

/* initialise the four known base terms */
static void init_base_terms(void)
{
    if (cached >= 4) return;                       /* already initialised */

    ensure_capacity(4);

    if (cached < 1) { cache[0] = (seq_t)2;  cached = 1; }
    if (cached < 2) { cache[1] = (seq_t)5;  cached = 2; }
    if (cached < 3) { cache[2] = (seq_t)37; cached = 3; }
    if (cached < 4) { cache[3] = (seq_t)82; cached = 4; }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for a(0) */

    /* make sure the cache can hold up to a(n) */
    ensure_capacity((size_t)n);

    /* initialise first four terms if not already in cache */
    init_base_terms();

    /* compute any missing terms up to n */
    if (cached < (size_t)n) {
        for (size_t i = cached + 1; i <= (size_t)n; ++i) {
            /* recurrence applies for i ≥ 5, but i will be ≥5 here */
            seq_t term_n2 = cache[i - 3];   /* a(i-2)  stored at index i-3  */
            seq_t term_n4 = cache[i - 5];   /* a(i-4)  stored at index i-5  */
            cache[i - 1] = (seq_t)16 * term_n2 - term_n4;
        }
        cached = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
