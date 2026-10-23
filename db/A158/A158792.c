/*
 * A158792  Number of n-colorings of the Shrikhande graph.
 * https://oeis.org/A158792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{16} Δ^k[0] * C(n,k) where Δ^k[0] are the
 *            forward‑differences of the first 17 values of the chromatic
 *            polynomial of the Shrikhande graph
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 18, u128 n <= 258,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first 17 values of the sequence: a(0) … a(16) */
    static const seq_t init[17] = {
        0ULL,                                   /* a(0) */
        0ULL,                                   /* a(1) */
        0ULL,                                   /* a(2) */
        0ULL,                                   /* a(3) */
        240ULL,                                 /* a(4) */
        332400ULL,                              /* a(5) */
        105480720ULL,                           /* a(6) */
        7787433360ULL,                          /* a(7) */
        233556076320ULL,                        /* a(8) */
        3881511113760ULL,                       /* a(9) */
        42569914312800ULL,                      /* a(10) */
        342908176422240ULL,                     /* a(11) */
        2176503575487120ULL,                    /* a(12) */
        11428762115181840ULL,                   /* a(13) */
        51423745096804080ULL,                   /* a(14) */
        203533610650297200ULL,                  /* a(15) */
        722989464927562560ULL                   /* a(16) */
    };

    static seq_t deltas[17];
    static bool ready = false;

    if (!ready) {
        seq_t temp[17];
        for (int i = 0; i < 17; ++i) temp[i] = init[i];

        /* compute forward differences Δ^k[0] */
        for (int d = 0; d < 17; ++d) {
            deltas[d] = temp[0];
            int remaining = 16 - d;
            for (int j = 0; j < remaining; ++j) {
                temp[j] = temp[j + 1] - temp[j];
            }
        }
        ready = true;
    }

    seq_t result = 0;

    /* evaluate Σ Δ^k[0] * C(n,k)  (k = 0 … 16) */
    for (idx_t k = 0; k < 17; ++k) {
        if (n < k) break;                /* C(n,k) = 0 */
        seq_t bin = 1;
        for (idx_t i = 1; i <= k; ++i) {
            bin = (bin * (n - i + 1)) / (seq_t)i;
        }
        result = result + bin * deltas[k];
    }

    return result;
}
