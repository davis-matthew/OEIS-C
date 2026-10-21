/*
 * A212150  Number of (w,x,y,z) with all terms in {1,...,n} and w*x+y*z<=n^2.
 * https://oeis.org/A212150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) with 1≤w,x,y,z≤n and w*x + y*z ≤ n^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1) {
        return 0;
    }

    /* N = n^2, maximum possible product of two variables */
    idx_t N = n * n;

    /* Allocate arrays of size N+1.
       cnt holds how many pairs (w,x) yield product p.
       Use unsigned short (16 bits) – counts never exceed n (≤10000). */
    size_t size = (size_t)N + 1;
    uint16_t *cnt = (uint16_t *)calloc(size, sizeof(uint16_t));
    if (!cnt) {
        return 0;   /* allocation failure */
    }

    /* Count all (w,x) products */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            cnt[(size_t)(w * x)]++;
        }
    }

    /* initial cumulative sum: sum of cnt[p] for p <= N-1
       (the value needed for q = 1). */
    seq_t cumulative = 0;
    for (idx_t p = 0; p < N; ++p) {
        cumulative += (seq_t)cnt[(size_t)p];
    }

    /* total number of quadruples */
    seq_t total = 0;

    /* iterate over product q = y*z */
    for (idx_t q = 1; q <= N; ++q) {
        if (cnt[(size_t)q] != 0) {
            total += (seq_t)cnt[(size_t)q] * cumulative;
        }
        /* update cumulative for next q:
           subtract cnt[N - q] which is no longer allowed when q increases. */
        if (q < N) {
            cumulative -= (seq_t)cnt[(size_t)(N - q)];
        }
    }

    free(cnt);
    return total;
}
