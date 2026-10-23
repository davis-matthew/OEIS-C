/*
 * A043621  Numbers whose base-7 representation has exactly 6 runs.
 * https://oeis.org/A043621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gen_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose base‑7 representation has exactly six runs */
#include <stdbool.h>
#include "../utils/gen_seq.h"

/* Binomial coefficient C(m,5) using 128‑bit arithmetic */
static unsigned __int128 binom5(idx_t m) {
    if (m < 5) return 0;
    unsigned __int128 r = 1;
    r *= m;          /* m */
    r *= (m - 1);    /* m*(m-1) */
    r *= (m - 2);    /* m*(m-1)*(m-2) */
    r *= (m - 3);    /* m*(m-1)*(m-2)*(m-3) */
    r *= (m - 4);    /* m*(m-1)*(m-2)*(m-3)*(m-4) */
    r /= 120;        /* divide by 5! = 120 */
    return r;
}

seq_t a(idx_t n) {
    /* Find the length L of the n‑th number */
    idx_t cum = 0;
    idx_t L = 6;
    idx_t offset = 0;
    while (1) {
        unsigned __int128 comb = binom5(L - 1);                 /* C(L-1,5) */
        unsigned __int128 count128 = comb * (unsigned __int128)46656; /* 6*6^5 = 46656 */
        if (count128 >= (unsigned __int128)(n - cum)) {
            offset = n - cum;
            break;
        }
        cum += (idx_t)count128;
        ++L;
    }

    /* Generate the offset‑th number of length L */
    seq_t result = 0;
    idx_t counter = 0;
    bool done = false;
    gen_seq(0, 0, 5, 0, L, &offset, &counter, &result, &done);
    return result;
}
