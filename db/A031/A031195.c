/*
 * A031195  Periods of sum of 6th powers of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = length of the eventual period reached by iterating
 *
 *   f(x) = sum of 6th powers of the decimal digits of x
 *
 * The function returns the size of the cycle that the sequence
 * (x, f(x), f(f(x)), …) eventually enters, for a given starting
 * value n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Precomputed 6th powers of digits 0…9. */
static const idx_t pow6[10] = {0, 1, 64, 729, 4096, 15625,
                               46656, 117649, 262144, 531441};

/* Maximum possible value of f(x) for any 64‑bit integer. */
#define MAX_SUM 10628820ULL   /* 20 * 531441 */

static unsigned char *a031195_cache = NULL;  /* cache of cycle lengths */

/* Sum of the 6th powers of the decimal digits of x. */
static inline idx_t sum6(idx_t x)
{
    idx_t s = 0;
    while (x > 0) {
        s += pow6[x % 10];
        x /= 10;
    }
    return s;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use. */
    if (!a031195_cache) {
        a031195_cache = (unsigned char *)calloc(MAX_SUM + 1, sizeof(unsigned char));
    }

    /* If n is within the cache range and already computed, return it. */
    if (n <= MAX_SUM && a031195_cache[n]) {
        return (seq_t)a031195_cache[n];
    }

    /* small local path storage; 2000 is ample for typical transient lengths. */
    idx_t path[2048];
    size_t len = 0;
    idx_t cur = n;
    seq_t cycle_len = 0;

    while (1) {
        /* Known cycle length through cache? */
        if (cur <= MAX_SUM && a031195_cache[cur]) {
            cycle_len = (seq_t)a031195_cache[cur];
            break;
        }

        /* Detect repetition in the current path. */
        size_t i;
        for (i = 0; i < len; ++i) {
            if (path[i] == cur) {
                cycle_len = (seq_t)(len - i);
                break;
            }
        }
        if (i < len) {
            /* Repeated value found; cycle length determined. */
            break;
        }

        /* Append current value to path and advance. */
        if (len < sizeof(path) / sizeof(path[0])) {
            path[len++] = cur;
        } else {
            /* Should not happen for the intended application. */
            /* Fallback: treat as cycle of length 1. */
            cycle_len = 1;
            break;
        }
        cur = sum6(cur);
    }

    /* Cache the cycle length for all values in the path that are within range. */
    for (size_t j = 0; j < len; ++j) {
        idx_t v = path[j];
        if (v <= MAX_SUM && !a031195_cache[v]) {
            a031195_cache[v] = (unsigned char)cycle_len;
        }
    }

    /* If the initial value itself was within range and cached now, cache it too. */
    if (n <= MAX_SUM && !a031195_cache[n]) {
        a031195_cache[n] = (unsigned char)cycle_len;
    }

    return cycle_len;
}
