/*
 * A049194  Number of digits in n-th term of A001387.
 * https://oeis.org/A049194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) (n ≥ 7) Initial values: a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=6, a(5)=8, a(6)=12
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

/* initial values – index 0 is unused to keep the array 1‑based */
static const seq_t a_init[7] = {
    0,      /* a(0) – unused */
    1,      /* a(1) */
    2,      /* a(2) */
    3,      /* a(3) */
    6,      /* a(4) */
    8,      /* a(5) */
    12      /* a(6) */
};

/* cache of computed terms, 1‑based.  cache[0] is unused. */
static seq_t *a_cache   = NULL;   /* pointer to the array */
static idx_t  a_size    = 0;      /* current cache size (largest n stored) */

/* Ensure that a_cache contains values up to n (inclusive).  If
   a_cache is NULL or too small, allocate a larger block and
   compute the missing terms. */
static void
ensure_cache(idx_t n)
{
    if (n <= a_size)          /* already cached */
        return;

    /* Allocate a block large enough for 0..n indices */
    size_t new_cap = (size_t)n + 1;          /* +1 for index 0 */
    seq_t *new_ptr;

#if defined(__cplusplus)
    /* In C++ the cast is required. */
    new_ptr = reinterpret_cast<seq_t*>(realloc(a_cache, new_cap * sizeof(seq_t)));
#else
    /* In C the cast is optional but harmless. */
    new_ptr = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
#endif

    if (!new_ptr) {
        /* Allocation failed – abort; the caller will not recover. */
        abort();
    }

    a_cache = new_ptr;

    /* Fill in the newly allocated part.  For indices 1..6 use a_init;
       for n ≥ 7 compute via the recurrence. */
    for (idx_t i = a_size + 1; i <= n; ++i) {
        if (i <= 6) {
            a_cache[i] = a_init[i];
        } else {
            /* a(i) = a(i‑1) + a(i‑2) – a(i‑5)  (n ≥ 7) */
            a_cache[i] = a_cache[i-1] + a_cache[i-2] - a_cache[i-5];
        }
    }

    a_size = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)           /* the sequence is 1‑based; a(0) is undefined – return 0 */
        return 0;

    ensure_cache(n);
    return a_cache[n];
}
