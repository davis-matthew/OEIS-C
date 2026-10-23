/*
 * A152621  a(n)=8*a(n-1)-6*a(n-2), n>1 ; a(0)=1, a(1)=2.
 * https://oeis.org/A152621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 6*a(n-2) ; a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* A fixed cache large enough for the typical n range (≈10000). */
    /* For larger n the sequence is computed on the fly. */
    enum { MAX_CACHE = 50000 };          /* safe upper bound for most tests */

    static seq_t cache[MAX_CACHE + 1];
    static idx_t max_cached = 0U;       /* highest index stored in cache */

    /* Initialise the base cases the first time. */
    if (max_cached == 0U) {
        cache[0] = 1;
        cache[1] = 2;
        max_cached = 1;
    }

    /* If the requested index is cached, return it. */
    if (n <= max_cached) {
        return cache[n];
    }

    /* When n is within the cache range, extend the cache. */
    if (n <= MAX_CACHE) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t cur;
            if (sizeof(seq_t) <= 8U) {
                /* Use 128‑bit arithmetic to avoid overflow on built‑in types. */
                unsigned __int128 t = (unsigned __int128)cache[i-1] * 8ULL
                                    - (unsigned __int128)cache[i-2] * 6ULL;
                cur = (seq_t)t;
            } else {
                cur = cache[i-1] * 8 - cache[i-2] * 6;
            }
            cache[i] = cur;
        }
        max_cached = n;
        return cache[n];
    }

    /* For indices beyond the cache, compute the term without storing. */
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 2;   /* a(1) */
    seq_t cur = prev1; /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        if (sizeof(seq_t) <= 8U) {
            unsigned __int128 t = (unsigned __int128)prev1 * 8ULL
                                - (unsigned __int128)prev2 * 6ULL;
            cur = (seq_t)t;
        } else {
            cur = prev1 * 8 - prev2 * 6;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
