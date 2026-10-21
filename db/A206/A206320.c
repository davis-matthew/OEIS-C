/*
 * A206320  Number of (n+1) X 2 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three clockwise edge increases.
 * https://oeis.org/A206320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 5*a(n-2) - 26*a(n-3) + 42*a(n-4) + 19*a(n-5) +
 *            42*a(n-6) + 14*a(n-7) Initial values (1 ≤ n ≤ 8): a(1)=256,
 *            a(2)=1344, a(3)=10504, a(4)=83088, a(5)=673124, a(6)=5432884,
 *            a(7)=43848120, a(8)=353716772
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0; /* number of allocated slots (cache[0] unused) */

    #ifdef __cplusplus
        /* C++ path: use new[] / delete[] so constructors are invoked */
        if (n < 1) return seq_t(0);

        const size_t req = (size_t)n + 1; /* need indices 0 .. n */
        if (req > cap) {
            seq_t *new_cache = new seq_t[req];
            /* copy old values */
            for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            /* initialize new entries */
            static const seq_t init_vals[9] = {
                seq_t(0),          /* unused */
                seq_t(256),        /* a(1) */
                seq_t(1344),       /* a(2) */
                seq_t(10504),      /* a(3) */
                seq_t(83088),      /* a(4) */
                seq_t(673124),     /* a(5) */
                seq_t(5432884),    /* a(6) */
                seq_t(43848120),   /* a(7) */
                seq_t(353716772)   /* a(8) */
            };
            for (size_t i = cap; i < req; ++i) {
                if (i <= 8) {
                    new_cache[i] = init_vals[i];
                } else {
                    new_cache[i] =  seq_t(9)*new_cache[i-1]
                                   - seq_t(5)*new_cache[i-2]
                                   - seq_t(26)*new_cache[i-3]
                                   + seq_t(42)*new_cache[i-4]
                                   + seq_t(19)*new_cache[i-5]
                                   + seq_t(42)*new_cache[i-6]
                                   + seq_t(14)*new_cache[i-7];
                }
            }
            delete[] cache;
            cache = new_cache;
            cap = req;
        } else if (n <= (idx_t)(cap-1) && cache[(size_t)n] != seq_t(0)) {
            /* value already computed */
        } else {
            /* need to compute from cap upto n (only when cache not fully populated) */
            static const seq_t init_vals[9] = {
                seq_t(0),          /* unused */
                seq_t(256),        /* a(1) */
                seq_t(1344),       /* a(2) */
                seq_t(10504),      /* a(3) */
                seq_t(83088),      /* a(4) */
                seq_t(673124),     /* a(5) */
                seq_t(5432884),    /* a(6) */
                seq_t(43848120),   /* a(7) */
                seq_t(353716772)   /* a(8) */
            };
            for (size_t i = cap; i <= (size_t)n; ++i) {
                if (i <= 8) {
                    cache[i] = init_vals[i];
                } else {
                    cache[i] =  seq_t(9)*cache[i-1]
                               - seq_t(5)*cache[i-2]
                               - seq_t(26)*cache[i-3]
                               + seq_t(42)*cache[i-4]
                               + seq_t(19)*cache[i-5]
                               + seq_t(42)*cache[i-6]
                               + seq_t(14)*cache[i-7];
                }
            }
            cap = (size_t)n + 1;
        }
        return cache[(size_t)n];
    #else
        /* C path: use malloc/realloc; works only when seq_t is a plain integer type */
        if (n < 1) return 0;

        size_t req = (size_t)n + 1; /* need indices 0 .. n */
        if (req > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, req * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed */
            cache = tmp;
            static const seq_t init_vals[9] = {
                0ULL,          /* unused */
                256ULL,        /* a(1) */
                1344ULL,       /* a(2) */
                10504ULL,      /* a(3) */
                83088ULL,      /* a(4) */
                673124ULL,     /* a(5) */
                5432884ULL,    /* a(6) */
                43848120ULL,   /* a(7) */
                353716772ULL   /* a(8) */
            };
            for (size_t i = cap; i <= (size_t)n; ++i) {
                if (i <= 8) {
                    cache[i] = init_vals[i];
                } else {
                    cache[i] = 9*cache[i-1] - 5*cache[i-2] - 26*cache[i-3]
                               + 42*cache[i-4] + 19*cache[i-5]
                               + 42*cache[i-6] + 14*cache[i-7];
                }
            }
            cap = req;
        }
        return cache[(size_t)n];
    #endif
}
