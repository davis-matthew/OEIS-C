/*
 * A088235  Total number of digits (in base 10) in all preceding terms in the
 *          sequence.
 * https://oeis.org/A088235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=0, a(1)=1; a(n+1) = a(n) + digits(a(n)), digits(x) =
 *            floor(log10(x))+1 (digits(0)=1)
 * Range:     u8 n <= 106, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Cache of previously computed terms: a[0 .. len-1]  */
static seq_t *cache = NULL;      /*  sequence values  */
static idx_t  cap   = 0;         /*  current capacity  */
static idx_t  len   = 0;         /*  number of stored terms  */

/*  Return the number of decimal digits of a non‑negative seq_t value.   */
static seq_t digit_count_seq_t(seq_t v)
{
    if (v == (seq_t)0) return (seq_t)1;
    unsigned cnt = 0;
    seq_t t = v;
    while (t > (seq_t)0) {
        t /= (seq_t)10;
        ++cnt;
    }
    return (seq_t)cnt;
}

/*  Grow the cache so that it can hold indices up to n (inclusive).   */
static void ensure_cache_cap(idx_t n)
{
    /*  Need capacity for n+1 elements (indices 0..n).   */
    if ((idx_t)(n + 1) <= cap) return;

    idx_t new_cap = (cap == 0) ? 16 : cap * 2;
    if (new_cap <= n + 1) new_cap = n + 1;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache) return;           /*  allocation failure – caller will get garbage; system values suffice  */
    for (idx_t i = 0; i < len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cap   = new_cap;
}

/*  Compute a(n) for any n ≥ 0.  Calls may arrive in arbitrary order.  */
seq_t a(idx_t n)
{
    if (len == 0) {
        ensure_cache_cap(n);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;   /*  a(1) = 1  */
        len = 2;
    } else {
        ensure_cache_cap(n);
        if (cache[1] == 0) { /*  in case the cache was cleared externally  */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            len = 2;
        }
    }

    for (idx_t i = len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev + digit_count_seq_t(prev);
    }
    len = n + 1;  /*  assure the cache size record is accurate  */
    return cache[n];
}
