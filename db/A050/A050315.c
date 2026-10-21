/*
 * A050315  Main diagonal of A050314.
 * https://oeis.org/A050315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000110(popcount(n)) (Bell number of the popcount of n)
 * Range:     u8 n <= 126, u16 n <= 1022, u32 n <= 9352, u64 n <= 9352,
 *            u128 n <= 9352, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t bell[65];
static bool bell_inited = false;

static void init_bell(void) {
    if (bell_inited) return;
    bell[0] = (seq_t)1;
    for (idx_t n = 0; n < 64; ++n) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;
        for (idx_t k = 0; k <= n; ++k) {
            if (k == 0) {
                bin = (seq_t)1;
            } else {
                bin = bin * ((seq_t)(n - k + 1)) / ((seq_t)k);
            }
            sum += bin * bell[k];
        }
        bell[n + 1] = sum;
    }
    bell_inited = true;
}

seq_t a(idx_t n) {
    init_bell();
    idx_t pc = __builtin_popcountll((unsigned long long)n);
    return bell[pc];
}
