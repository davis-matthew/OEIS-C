/*
 * A026177  For n >= 2, let h=floor(n/2), L=n-h, R=n+h; then a(R)=n if n odd
 *          or a(L) already defined, otherwise a(L)=n.
 * https://oeis.org/A026177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2 let h=floor(n/2), L=n−h, R=n+h. Then a(R)=n if n
 *            odd or a(L) already defined, otherwise a(L)=n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes – valid in both C11 and C++17 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Forward declaration of helper.  Its definition follows after `a`. */
static void ensure_cache(idx_t need);

/* Sequence values are stored in a static cache indexed by n. */
static seq_t *seq_cache = NULL;      /* array of seq_t, 0‑based.            */
static idx_t cache_size = 0;         /* number of allocated elements.       */
static idx_t processed_upto = 0;     /* last i that has been processed.     */

static void
ensure_cache(idx_t need)
/* Allocate the cache to hold indices up to `need` (inclusive).  The
   cache is a contiguous array of seq_t.  New elements are zero‑initialised. */
{
    if (need < cache_size) return;          /* already large enough */

    idx_t new_size = need + 1;               /* indices 0..need inclusive */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);     /* OOM */

    /* Zero‑initialise the newly added part. */
    memset(new_cache + cache_size, 0,
           (new_size - cache_size) * sizeof(seq_t));

    seq_cache = new_cache;
    cache_size = new_size;

    /* Base case a(1)=1 if not already set. */
    if (cache_size > 1 && seq_cache[1] == 0)
        seq_cache[1] = (seq_t)1;
}

seq_t a(idx_t n)
/* Return the nth term of OEIS sequence A026177.
   `seq_cache` is filled lazily on demand and can be queried in any order. */
{
    if (n == 0) return (seq_t)0;          /* undefined for the sequence */

    /* Ensure the cache can store all indices up to 3*n, which is the
       maximum index that may be written when processing values up to i=2*n. */
    ensure_cache(3 * n);

    /* Process all i from processed_upto+1 up to 2*n.  Each i sets either
       a(L) or a(R), according to the definition. */
    if (2 * n > processed_upto) {
        for (idx_t i = processed_upto + 1; i <= 2 * n; ++i) {
            idx_t h = i >> 1;                 /* floor(i/2)          */
            idx_t L = i - h;                  /* L = i - h           */
            idx_t R = i + h;                  /* R = i + h           */

            /* When i is odd, we always set a(R)=i.  When i is even and
               a(L) is already defined, we also set a(R)=i; otherwise
               (i even and a(L) undefined) we set a(L)=i. */
            if ((i & 1U) || (seq_cache[L] != 0)) {
                if (seq_cache[R] == 0)
                    seq_cache[R] = (seq_t)i;
            } else {
                if (seq_cache[L] == 0)
                    seq_cache[L] = (seq_t)i;
            }
        }
        processed_upto = 2 * n;
    }

    return seq_cache[n];
}
