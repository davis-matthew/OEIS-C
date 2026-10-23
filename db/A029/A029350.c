/*
 * A029350  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-6) + a(n-7) - a(n-10) - a(n-13) - a(n-15) *
 *            - a(n-18) + a(n-21) + a(n-22) + a(n-24) - a(n-28) * Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=0 * Part sizes: 4, 6, 7, 11
 * Range:     u8 n <= 127, u16 n <= 885, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* At most 20001 entries are needed for n up to about 10000.
       The static array is zero‑initialised for POD types and default‑constructed
       for user defined types, so the correct initial value a(0)=1 can be set
       once and reused across all calls. */
    static seq_t cache[20001];
    static idx_t cached_up_to = 0;

    /* Initialise the singleton caching structure */
    if (cached_up_to == 0 && cache[0] != (seq_t)1) {
        cache[0] = (seq_t)1;  /* a(0) = 1 */
    }

    /* If the requested value is beyond the current cached range, extend the cache */
    if (n > cached_up_to) {
        /* Ensure we do not overrun the fixed buffer */
        if (n >= sizeof(cache)/sizeof(cache[0])) {
            /* For n larger than the buffer we compute the term directly
               using the recurrence, without caching beyond the buffer size. */
            for (idx_t i = cached_up_to + 1; i <= n; ++i) {
                seq_t sum = 0;
                if (i >= 4) sum += cache[i - 4];
                if (i >= 6) sum += cache[i - 6];
                if (i >= 7) sum += cache[i - 7];
                if (i >= 10) sum -= cache[i - 10];
                if (i >= 13) sum -= cache[i - 13];
                if (i >= 15) sum -= cache[i - 15];
                if (i >= 18) sum -= cache[i - 18];
                if (i >= 21) sum += cache[i - 21];
                if (i >= 22) sum += cache[i - 22];
                if (i >= 24) sum += cache[i - 24];
                if (i >= 28) sum -= cache[i - 28];
                /* Store into the cache if still within bounds,
                   otherwise discard (only keep the current result). */
                if (i < sizeof(cache)/sizeof(cache[0]))
                    cache[i] = sum;
                if (i == n) return sum;
            }
            return 0; /* Should not reach here */
        }

        /* Compute all entries up to n */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t sum = 0;
            if (i >= 4) sum += cache[i - 4];
            if (i >= 6) sum += cache[i - 6];
            if (i >= 7) sum += cache[i - 7];
            if (i >= 10) sum -= cache[i - 10];
            if (i >= 13) sum -= cache[i - 13];
            if (i >= 15) sum -= cache[i - 15];
            if (i >= 18) sum -= cache[i - 18];
            if (i >= 21) sum += cache[i - 21];
            if (i >= 22) sum += cache[i - 22];
            if (i >= 24) sum += cache[i - 24];
            if (i >= 28) sum -= cache[i - 28];
            cache[i] = sum;
        }
        cached_up_to = n;
    }
    return cache[n];
}
