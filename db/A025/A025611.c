/*
 * A025611  Numbers of form 2^i*9^j, with i, j >= 0.
 * https://oeis.org/A025611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 182, u64 n <= 687,
 *            u128 n <= 2668, big any n
 * Validated: standard, pass, 64 terms through n = 8864 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A025611 – Numbers of the form 2^i * 9^j (i,j ≥ 0),
    sorted in increasing order.
    The function generates the list by keeping two running indices
    and multiplying the earlier terms by 2 and 9 respectively.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/*  Function prototype supplied by the build environment:
        seq_t a(idx_t n)
    where:
        - idx_t is an unsigned integer type (e.g. uint64_t).
        - seq_t is an unsigned integer type or an arbitrary‑precision class.
*/

seq_t a(idx_t n)
{
    /* static cache of generated values  */
    static seq_t *cache = NULL;
    static size_t size = 0;       /* number of values stored          */
    static size_t cap  = 0;       /* allocated capacity               */
    static idx_t  i2   = 0;       /* index for multiplication by 2    */
    static idx_t  i9   = 0;       /* index for multiplication by 9    */

    /* initialize cache on first call */
    if (cache == NULL) {
        cap   = 32;                          /* initial capacity */
        cache = seq_alloc(cap);              /* allocates array of seq_t */
        cache[0] = (seq_t)1;                 /* a(1) = 1 */
        size = 1;
        i2   = 0;
        i9   = 0;
    }

    /* generate terms until we have at least n entries */
    while (size < n) {
        seq_t cand2 = mul_small(cache[i2], 2);
        seq_t cand9 = mul_small(cache[i9], 9);

        int cmp = seq_cmp(&cand2, &cand9);
        seq_t next;
        if (cmp < 0) {
            next = cand2;
        } else if (cmp > 0) {
            next = cand9;
        } else {              /* cand2 == cand9 */
            next = cand2;     /* either value is fine */
        }

        /* grow the cache if necessary */
        if (size == cap) {
            size_t new_cap = cap * 2;
            if (new_cap < size + 1) new_cap = size + 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap   = new_cap;
        }

        cache[size++] = next;             /* store the next value */

        /* advance the pointers that contributed to next */
        if (seq_cmp(&cand2, &next) == 0) i2++;
        if (seq_cmp(&cand9, &next) == 0) i9++;
    }

    return cache[n - 1];
}
