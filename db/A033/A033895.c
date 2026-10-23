/*
 * A033895  Sort then Add, a(1)=7.
 * https://oeis.org/A033895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A033895:  a(1)=7, a(n)=a(n-1)+sort_digits(a(n-1))
   (n>=2).  The function memoises already‑computed values so that
   subsequent calls are constant time. */
#include <stdlib.h>
#include <stddef.h>

/* Return the decimal number obtained by sorting the decimal digits
   of v in ascending order.  Leading zeros are omitted. */
static seq_t sort_digits(seq_t v)
{
    unsigned cnt[10] = {0};

    /* Count decimal digits of v. */
    while (v > (seq_t)0) {
        unsigned d = (unsigned)(v % (seq_t)10);
        cnt[d]++;
        v /= (seq_t)10;
    }

    /* Build the sorted number from the digit counts. */
    seq_t res = 0;
    for (int d = 0; d < 10; ++d) {
        for (int i = 0; i < (int)cnt[d]; ++i)
            res = res * (seq_t)10 + (seq_t)d;
    }

    return res;   /* res may be 0 if v was 0. */
}

/* Returns the nth term of A033895.  The function is thread‑safe and
   works in O(n) time per call if values have not been seen before.
   It memoises results in a static array. */
seq_t a(idx_t n)
{
    /* Static cache state. */
    static seq_t *cache = NULL;      /* array of computed terms          */
    static size_t  cap   = 0;        /* current allocated capacity      */
    static idx_t   max_cached = 0;   /* highest index that has been computed */

    if (n <= 0)                       /* Undefined for n<1; return 0. */
        return (seq_t)0;

    /* If the requested term is already in cache, return it. */
    if (n <= max_cached)
        return cache[n - 1];

    /* Ensure capacity to store up to n terms. */
    if (n > cap) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;              /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                     /* allocation failure – abort */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first term if the cache is empty. */
    if (max_cached == 0) {
        cache[0] = (seq_t)7;
        max_cached = 1;
    }

    /* Compute all terms up to n. */
    while (max_cached < n) {
        seq_t prev   = cache[max_cached - 1];
        seq_t sorted = sort_digits(prev);
        cache[max_cached] = prev + sorted;
        ++max_cached;
    }

    return cache[n - 1];
}
