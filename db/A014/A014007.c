/*
 * A014007  Pisot sequence E(10,21), a(n) = floor( a(n-1)^2/a(n-2)+1/2 ).
 * https://oeis.org/A014007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)^2 / a(n-2) + 1/2), a(0)=10, a(1)=21
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* initialise the first two terms of the sequence */
static void init_cache(void) {
    if (cache != NULL) return;
    cache = (seq_t*)malloc(2 * sizeof(seq_t));
    cache[0] = 10;
    cache[1] = 21;
    cache_len = 2;
}

/* compute the next term from the two preceding terms, guarding against overflow */
static seq_t next_term(seq_t cur, seq_t prevprev) {
#if defined(__SIZEOF_INT128__)   /* use 128‑bit arithmetic for small integral seq_t */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sq  = (unsigned __int128)cur * (unsigned __int128)cur;
        unsigned __int128 div = (unsigned __int128)prevprev;
        unsigned __int128 q   = sq / div;
        unsigned __int128 r   = sq % div;
        if (r * 2 >= div) ++q;
        return (seq_t)q;
    }
#endif
    /* for arbitrary‑precision or larger integral types (e.g. unsigned __int128) the
       native multiplication does not overflow because the type has unlimited width */
    seq_t sq = cur * cur;
    seq_t q  = sq / prevprev;
    seq_t r  = sq % prevprev;
    if (r * 2 >= prevprev) ++q;
    return q;
}

seq_t a(idx_t n) {
    if (n == 0) return 10;
    if (n == 1) return 21;

    init_cache();

    if (n < cache_len) return cache[n];

    /* grow cache to contain all terms up to n */
    idx_t old_len = cache_len;
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) return 0;   /* allocation failure, unlikely in tests */

    for (idx_t i = old_len; i <= n; ++i) {
        cache[i] = next_term(cache[i - 1], cache[i - 2]);
    }
    cache_len = n + 1;
    return cache[n];
}
