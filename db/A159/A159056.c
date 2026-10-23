/*
 * A159056  Number of n-colorings of the Clebsch Graph.
 * https://oeis.org/A159056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{16} binomial(n,k) * Δ^k a(0) (where Δ^k a(0) are
 *            the kth forward differences of the first 17 terms)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 18, u128 n <= 258,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* pre‑computed first 17 terms of the sequence */
    static const seq_t init_vals[17] = {
        0ULL, 0ULL, 0ULL, 0ULL, 28560ULL,
        18277200ULL, 1925512560ULL, 71729634480ULL,
        1389478980960ULL, 17119717947360ULL,
        151217975815200ULL, 1034656343471520ULL,
        5783087147848560ULL, 27431854405990320ULL,
        113598910323858960ULL, 419654992044834000ULL,
        1406448998999378880ULL
    };

    /* Δ^k a(0) for k = 0..16 */
    static seq_t diff[17];
    static bool ready = false;

    if (!ready) {
        seq_t t[17];
        for (int i = 0; i < 17; ++i) t[i] = init_vals[i];
        diff[0] = t[0];
        for (int k = 1; k <= 16; ++k) {
            for (int i = 0; i < 17 - k; ++i) {
                t[i] = t[i + 1] - t[i];
            }
            diff[k] = t[0];
        }
        ready = true;
    }

    seq_t ans = 0;
    seq_t bin = 1; /* binomial(n,0) */

    for (int k = 0; k <= 16; ++k) {
        if (n < (idx_t)k) break;          /* binomial(n,k)=0 for k>n */
        ans += bin * diff[k];
        if (k == 16) break;
        bin = bin * (seq_t)(n - k) / (k + 1);  /* next binomial coefficient */
    }
    return ans;
}
