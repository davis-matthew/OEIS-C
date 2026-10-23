/*
 * A189671  Partial sums of A189668.
 * https://oeis.org/A189671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s([(n+2)/3]) + [(n+1)/3] - s([(n+1)/3]), with s(0)=0,
 *            s(1)=0. This is the partial sum of A189668
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef CACHE_CAP
#define CACHE_CAP 2000000  /* Enough for all expected n in tests */
#endif

static seq_t cache[CACHE_CAP];
static idx_t max_computed = 1;  /* cache[0] and cache[1] are already 0 */

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* a(1) = 0; recurrence base */
        return 0;
    }

    if (n >= CACHE_CAP) {
        /* Very unlikely with given constraints – compute recursively */
        idx_t a1 = (n + 2) / 3;
        idx_t b1 = (n + 1) / 3;
        return a(a1) + (seq_t)b1 - a(b1);
    }

    /* Compute values up to n if not already done */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t a1 = (i + 2) / 3;
            idx_t b1 = (i + 1) / 3;
            cache[i] = cache[a1] + (seq_t)b1 - cache[b1];
        }
        max_computed = n;
    }

    return cache[n];
}
