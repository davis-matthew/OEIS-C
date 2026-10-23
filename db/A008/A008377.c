/*
 * A008377  Crystal ball sequence for D_9 lattice.
 * https://oeis.org/A008377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{9} binom(n,k) * Δ^k a(0) where Δ^k a(0) are
 *            the first 10 forward differences of the sequence. The sequence
 *            is a polynomial of degree 9, so its 10th forward difference is
 *            zero. The first 10 terms of the sequence (a(0) … a(9)) are:
 *            1,145,4339,55171,416773,2218645, 9195511,31608967...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 154, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

static const seq_t init_seq[10] = {
    (seq_t)1,          /* a(0) */
    (seq_t)145,        /* a(1) */
    (seq_t)4339,       /* a(2) */
    (seq_t)55171,      /* a(3) */
    (seq_t)416773,     /* a(4) */
    (seq_t)2218645,    /* a(5) */
    (seq_t)9195511,    /* a(6) */
    (seq_t)31608967,   /* a(7) */
    (seq_t)94016137,   /* a(8) */
    (seq_t)249258777   /* a(9) */
};

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t delta[10];

    if (!init) {
        /* Compute forward differences Δ^k a(0), k = 0..9 */
        seq_t arr[10];
        for (int i = 0; i < 10; ++i)
            arr[i] = init_seq[i];
        delta[0] = arr[0];
        for (int k = 1; k < 10; ++k) {
            for (int i = 0; i < 10 - k; ++i)
                arr[i] = arr[i + 1] - arr[i];
            delta[k] = arr[0];
        }
        init = true;
    }

    /* For small n just return the precomputed value. */
    if (n < 10)
        return init_seq[n];

    /* Evaluate a(n) via the binomial summation. */
    seq_t result = (seq_t)0;
    for (int k = 0; k < 10; ++k) {
        result += binom(n, (idx_t)k) * delta[k];
    }
    return result;
}
