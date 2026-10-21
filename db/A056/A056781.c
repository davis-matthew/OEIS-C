/*
 * A056781  Prime powers such that the 4th power of the number of divisors is
 *          not smaller than the number itself.
 * https://oeis.org/A056781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = prime powers p^e such that (e+1)^4 >= p^e, sorted
 *            ascending
 * Range:     u8 n <= 19, u16 n <= 31, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   build_sequence, u128_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_sequence.h"
#include "../utils/u128_le.h"

/* The types seq_t and idx_t are supplied by the build system. */

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static storage for the sequence. */
    static unsigned __int128 seq[64];
    static int seq_count = 0;
    static bool built = false;

    if (!built) {
        build_sequence(seq, &seq_count);
        built = true;
    }

    if (n == 0 || n > (idx_t)seq_count)
        return (seq_t)0;          /* out‑of‑range */

    return (seq_t)seq[(int)(n - 1)];
}
