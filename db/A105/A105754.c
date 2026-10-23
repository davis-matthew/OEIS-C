/*
 * A105754  Lucas 8-step numbers.
 * https://oeis.org/A105754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..8} a(n-k), offset 1, a(1)=1, a(2)=3, a(3)=7,
 *            a(4)=15, a(5)=31, a(6)=63, a(7)=127, a(8)=255
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms of A105754.  It caches computed values
   statically so that calls may arrive in any order.  The cache size
   is sufficient for all n up to 10000 (as required by the runtime
   limits); it may be increased if larger n are needed.  */
seq_t a(idx_t n)
{
    /* Maximum n that can be handled without reallocating the cache. */
    /* 10000 is enough for the intended usage; this value can be
       changed safely. */
    enum { MAX_CACHE = 10000 };

    /* Use 1‑based indexing for the cache.  The first element cache[0]
       is unused. */
    static seq_t cache[MAX_CACHE + 1];
    static idx_t cached = 0;   /* highest index already computed */

    /* Initialise the first eight terms lazily on first entry. */
    if (cached == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)31;
        cache[6] = (seq_t)63;
        cache[7] = (seq_t)127;
        cache[8] = (seq_t)255;
        cached   = 8;
    }

    /* If the requested index is already cached, return it immediately. */
    if (n <= cached) {
        /* n is assumed to be >= 1; behaviour for n==0 is undefined,
           but we can return 8 to match the recurrence definition. */
        if (n == 0) return (seq_t)8;
        return cache[n];
    }

    /* The caller requested a term beyond the current cache size.
       Extend the cache up to n, or up to MAX_CACHE if n exceeds it. */
    idx_t limit = n;
    if (limit > MAX_CACHE) {
        /* In practice, n will stay below MAX_CACHE; if it does not,
           we recompute sequentially without caching beyond MAX_CACHE.
           This conserves memory while still giving the correct value. */
        limit = MAX_CACHE;
    }

    /* Compute the missing terms. */
    for (idx_t i = cached + 1; i <= limit; ++i) {
        seq_t sum = (seq_t)0;
        /* Sum the previous eight terms. */
        for (idx_t k = 1; k <= 8; ++k) {
            sum += cache[i - k];
        }
        cache[i] = sum;
    }

    if (n <= limit) {
        cached = limit;
        return cache[n];
    }

    /* n > MAX_CACHE: compute the requested term on the fly using the
       same recurrence but don't store it in the cache (to avoid
       exceeding the pre‑allocated size).  The value may be quite large
       but the computation is still O(n). */
    seq_t sum   = (seq_t)0;
    seq_t prev[8];
    /* Initialise prev with the last eight cached values. */
    for (idx_t k = 0; k < 8; ++k) {
        prev[k] = cache[limit - (7 - k)];
    }

    for (idx_t i = limit + 1; i <= n; ++i) {
        sum = (seq_t)0;
        for (idx_t k = 0; k < 8; ++k)
            sum += prev[k];
        /* Rotate the window. */
        for (idx_t k = 0; k < 7; ++k)
            prev[k] = prev[k + 1];
        prev[7] = sum;
    }
    return sum;
}
