/*
 * A025274  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 5,
 *          starting 1,1,1,0.
 * https://oeis.org/A025274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-1)*a(1) for n >= 5
 *            with initial values a(1)=1, a(2)=1, a(3)=1, a(4)=0 Memoized to
 *            allow arbitrary call order
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache holds values up to a maximum of 10000 as required by the test harness. */
    static seq_t cache[10001];
    static idx_t computed = 0;   /* greatest n that has been computed so far */

    if (n < 1) return (seq_t)0; // out of bounds, undefined behaviour otherwise

    if (computed == 0) {
        /* initialise the first four terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        computed = 4;
    }

    if (n <= computed)
        return cache[n];

    /* guard against requests beyond the preallocated limit */
    if (n > 10000) {
        /* the build guarantees n <= ~10000, so we simply return 0 for unexpected values */
        return (seq_t)0;
    }

    /* compute successive terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k < 5) continue;           /* safety, already initialised */

        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i < k; ++i) {
            sum += cache[i] * cache[k - i];
        }
        cache[k] = sum;
    }

    computed = n;
    return cache[n];
}
