/*
 * A087739  a(1)=1; a(2)=2; for n > 2, a(n) satisfies a(S(n))=n and a(k)=n-1
 *          for S(n-1) < k < S(n) where S(n) = a(1) + a(2) + ... + a(n).
 * https://oeis.org/A087739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2)=2; for n>2, a(n)=Golomb(n-1), where Golomb is the
 *            standard Golomb sequence defined by: g(1)=1,
 *            g(n)=1+g(n-g(g(n-1))) (n>1)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is memoised in a dynamically grown array which works
   with both integral seq_t types (C builds) and arbitrary‑precision
   seq_t classes (C++ builds). */

#include <stddef.h>
#ifdef __cplusplus
#include <new>
#else
#include <stdlib.h>
#endif

/* Forward declaration of globals used by ensure_golomb. */
static seq_t *golomb = NULL;      /* 1‑indexed array of Golomb values */
static size_t cap = 0;            /* allocated capacity (elements) */
static size_t max_cached = 0;     /* highest index computed */

/* Ensure the Golomb array is allocated and filled up to index 'need'. */
static void ensure_golomb(size_t need)
{
    if (need <= max_cached)
        return;

    /* Initial allocation if necessary. */
    if (golomb == NULL) {
        cap = 256;
#ifdef __cplusplus
        golomb = new seq_t[cap]();      /* value‑initialise all entries */
#else
        golomb = (seq_t *)calloc(cap, sizeof(seq_t));
#endif
        golomb[1] = 1;                  /* Golomb(1) = 1 */
        max_cached = 1;
    }

    /* Grow the array if 'need' exceeds current capacity. */
    if (need > cap) {
        size_t newcap = cap;
        while (newcap < need)
            newcap <<= 1;               /* double until sufficient */
#ifdef __cplusplus
        seq_t *newarr = new seq_t[newcap]();
#else
        seq_t *newarr = (seq_t *)calloc(newcap, sizeof(seq_t));
#endif
        /* Copy previously computed values. */
        for (size_t i = 1; i <= max_cached; ++i)
            newarr[i] = golomb[i];
#ifdef __cplusplus
        delete [] golomb;
#else
        free(golomb);
#endif
        golomb = newarr;
        cap = newcap;
    }

    /* Compute any remaining Golomb values up to 'need'. */
    for (size_t i = max_cached + 1; i <= need; ++i) {
        size_t prev_idx    = (size_t)golomb[i - 1];          /* g(i-1)   */
        size_t inner_idx   = (size_t)golomb[prev_idx];      /* g(g(i-1))*/
        golomb[i] = 1 + golomb[i - inner_idx];              /* recurrence */
    }
    max_cached = need;
}

/* Public sequence function expected by the framework. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* a(1)=1 */
    if (n == 2)
        return 2;          /* a(2)=2 */

    /* For n>=3 use shifted Golomb: a(n)=Golomb(n-1). */
    size_t idx = (size_t)(n - 1);
    ensure_golomb(idx);
    return golomb[idx];
}
