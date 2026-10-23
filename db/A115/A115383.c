/*
 * A115383  Row sums of Thue-Morse correlation triangle A115382.
 * https://oeis.org/A115383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} S1(k) * S2(n-k) where S1(k) = Σ_{j=0..k}
 *            t(j+1) (prefix sums of Thue‑Morse bits) and S2(m) = Σ_{j=0..m}
 *            t(j+1)*(-1)^{m-j} This implementation runs in O(n²) time and
 *            O(n) memory
 * Range:     u8 n <= 59, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute the parity of the 64‑bit integer x
   (parity = 1 if popcount(x) is odd, else 0). */
static inline uint64_t parity_uint64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_parityll(x);
#else
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1UL;
#endif
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    /* Allocate small integer arrays for S1 and S2. */
    /* They are safe for all n used in the tests (up to ~10 000). */
    size_t N = (size_t)n + 1U;
    uint64_t *S1 = (uint64_t *)malloc(N * sizeof(uint64_t));
    int64_t   *S2 = (int64_t   *)malloc(N * sizeof(int64_t));
    if (!S1 || !S2) {
        /* If memory allocation fails, abort: behaviour undefined. */
        exit(EXIT_FAILURE);
    }

    /* Build S1 and S2. */
    for (size_t i = 0; i < N; ++i) {
        uint64_t t = parity_uint64((uint64_t)(i + 1U));   /* t(i) = parity(popcount(i+1)) */
        if (i == 0) {
            S1[0] = t;
            S2[0] = (int64_t)t;
        } else {
            S1[i] = S1[i - 1] + t;
            S2[i] = (int64_t)t - S2[i - 1];
        }
    }

    /* Compute the convolution a(n). */
    seq_t result = 0;
    for (size_t k = 0; k <= n; ++k) {
        seq_t prod = (seq_t)S1[k] * (seq_t)S2[(size_t)n - k];
        result += prod;
    }

    /* Free the temporary arrays. */
    free(S1);
    free(S2);

    return result;
}
