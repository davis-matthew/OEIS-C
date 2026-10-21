/*
 * A235956  Number of (n+1) X (2+1) 0..1 arrays with the difference of the
 *          upper and lower median value of each 2 X 2 subblock in
 *          lexicographically nondecreasing order columnwise and nonincreasing
 *          rowwise.
 * https://oeis.org/A235956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8*a(n-1) - 16*a(n-2)
 *            - 22*a(n-3) + 105*a(n-4) - 66*a(n-5) - 120*a(n-6) + 176*a(n-7) -
 *            64*a(n-8) with initial values a(1)=50, a(2)=228, a(3)=928,
 *            a(4)=3574, a(5)=13346, a(6)=48848, a(7)=176420, a(8)=631130
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External link to the function as required. */
seq_t a(idx_t n)
{
    /* initial values for n = 1..8 */
    static const seq_t init[9] = {
        0,       /* unused index 0 */
        50,      /* a(1) */
        228,     /* a(2) */
        928,     /* a(3) */
        3574,    /* a(4) */
        13346,   /* a(5) */
        48848,   /* a(6) */
        176420,  /* a(7) */
        631130   /* a(8) */
    };

    /* fast‑path for the first eight terms */
    if (n <= 8) {
        return init[(size_t)n];
    }

    /* Static cache for memoisation (indices are 1 based). */
    static seq_t *buf = NULL;   /* holds a[1 …] */
    static size_t cap = 0;      /* capacity of buf (elements) */
    static size_t len = 0;      /* number of terms currently stored */

    /* Initialise cache on first use (already zeroed due to static). */
    if (len == 0) {
        /* allocate space for indices 0 … 8 (index 0 unused) */
        buf = (seq_t *)malloc(9 * sizeof(seq_t));
        if (!buf) return (seq_t)0;  /* out‑of‑memory: return 0 as sentinel */
        for (size_t i = 1; i <= 8; ++i) {
            buf[i] = init[i];
        }
        cap = 9;
        len = 8;                 /* we now have terms up to a(8) */
    }

    /* Grow and extend the cache to cover the requested index n. */
    if ((idx_t)len < n) {
        /* Double capacity until it fits n. */
        size_t newcap = cap;
        while (newcap <= (size_t)n) {
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out‑of‑memory */
        buf = tmp;
        cap = newcap;

        /* Compute new terms iteratively using the recurrence. */
        for (size_t k = len + 1; k <= (size_t)n; ++k) {
            seq_t term = 0;
            term += (seq_t)8   * buf[k-1];
            term -= (seq_t)16  * buf[k-2];
            term -= (seq_t)22  * buf[k-3];
            term += (seq_t)105 * buf[k-4];
            term -= (seq_t)66  * buf[k-5];
            term -= (seq_t)120 * buf[k-6];
            term += (seq_t)176 * buf[k-7];
            term -= (seq_t)64  * buf[k-8];
            buf[k] = term;
        }
        len = n;
    }

    return buf[(size_t)n];
}
