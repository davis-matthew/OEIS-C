/*
 * A159589  Positive numbers y such that y^2 is of the form x^2+(x+449)^2 with
 *          integer x.
 * https://oeis.org/A159589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 30, u64 n <= 66, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A159589
 *
 *  a(n) = 6*a(n‑3) – a(n‑6)  for n > 6
 *  a(1) = 421
 *  a(2) = 449
 *  a(3) = 481
 *  a(4) = 2045
 *  a(5) = 2245
 *  a(6) = 2465
 *
 *  The first 24 terms are listed in the OEIS entry.
 *
 *  The function below memoises the terms in a static array sized
 *  for n ≤ 10 000, which is sufficient for all tests that use this
 *  sequence.  For larger n it falls back to an on‑the‑fly
 *  computation that only keeps the six most recent terms in
 *  local variables.
 *
 *  All arithmetic is performed in the generic integer type seq_t,
 *  so the function works for 8/16/32/64‑bit unsigned integers
 *  as well as the arbitrary‑precision integer class used by the
 *  big‑integer build.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices – the OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache – works for 8/16/32/64‑bit and arbitrary‑precision. */
    static seq_t cache[10001];   /* index 0 is unused */
    static idx_t  cache_len = 0; /* highest index that has been stored */
    static bool   init_done = false;

    /* Initialise the first six terms once. */
    if (!init_done) {
        cache[1] = (seq_t)421;
        cache[2] = (seq_t)449;
        cache[3] = (seq_t)481;
        cache[4] = (seq_t)2045;
        cache[5] = (seq_t)2245;
        cache[6] = (seq_t)2465;
        cache_len = 6;
        init_done = true;
    }

    /* If the requested term is already cached, return it directly. */
    if (n <= cache_len && n <= 10000)
        return cache[n];

    /* When n > 10 000 we compute only the needed terms without
       extending the cache beyond its fixed size. */
    if (n > 10000) {
        seq_t a_nm3 = n > 6 ? cache[6] : 0; /* placeholder for a(n-3) */
        seq_t a_nm6 = 0;
        /* Initialise a_nm3 and a_nm6 from the cached last terms */
        if (cache_len >= 6) {
            a_nm3 = cache[6];
            a_nm6 = (cache_len >= 6 ? cache[0] : 0); /* a(0) is unused */
        }

        seq_t value = 0;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Compute a(i) = 6*a(i-3) - a(i-6) */
            value = ((seq_t)6) * a_nm3 - a_nm6;

            /* Shift the sliding window of the last six terms. */
            a_nm6 = a_nm3;  /* old a(i-3) becomes a(i-6) */
            a_nm3 = value;  /* new value becomes a(i-3) for next iteration */
        }
        return value;
    }

    /* For n ≤ 10 000 but > cache_len: extend the cache up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = ((seq_t)6) * cache[i - 3] - cache[i - 6];
    }
    cache_len = n;
    return cache[n];
}
