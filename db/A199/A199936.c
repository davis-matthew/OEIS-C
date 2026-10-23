/*
 * A199936  Total sum of Fibonacci parts in all partitions of n.
 * https://oeis.org/A199936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of Fibonacci parts (their values) in all integer
 *            partitions of n. The generating function is ∑_{n≥0} a(n)x^n =
 *            (∑_{i≥2} F_i x^{F_i}/(1−x^{F_i}))·∏_{j≥1} 1/(1−x^j)
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 96, u64 n <= 345,
 *            u128 n <= 1301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over partition sizes.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t sz = (size_t)(n + 1);
    seq_t *ways  = NULL;
    seq_t *sumFD = NULL;
    bool  *isFib = NULL;

#ifdef __cplusplus
    /* new[] with parentheses value‑initialises each element to 0 */
    ways  = new seq_t[sz]();
    sumFD = new seq_t[sz]();
    isFib = new bool[sz]();
#else
    ways  = calloc(sz, sizeof(seq_t));
    sumFD = calloc(sz, sizeof(seq_t));
    isFib = calloc(sz, sizeof(bool));
    if (!ways || !sumFD || !isFib) {
        free(ways); free(sumFD); free(isFib);
        return (seq_t)0;
    }
#endif

    /* Mark Fibonacci numbers up to n (1 occurs twice but is counted once). */
    unsigned long long f1 = 1, f2 = 1;
    if (n >= 1) isFib[1] = true;
    while (f2 <= n) {
        unsigned long long f3 = f1 + f2;
        if (f3 > n) break;
        if (f3 <= n) isFib[(size_t)f3] = true;
        f1 = f2;
        f2 = f3;
    }

    ways[0]  = 1;
    sumFD[0] = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t add = isFib[(size_t)k] ? (seq_t)k : (seq_t)0;
        for (idx_t s = k; s <= n; ++s) {
            seq_t w = ways[(size_t)(s - k)];
            ways[(size_t)s] += w;
            if (add)
                sumFD[(size_t)s] += sumFD[(size_t)(s - k)] + w * add;
            else
                sumFD[(size_t)s] += sumFD[(size_t)(s - k)];
        }
    }

    seq_t result = sumFD[(size_t)n];

#ifdef __cplusplus
    delete[] ways;
    delete[] sumFD;
    delete[] isFib;
#else
    free(ways);
    free(sumFD);
    free(isFib);
#endif

    return result;
}
