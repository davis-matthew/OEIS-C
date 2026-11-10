/*
 * A230298  a(n) = A010062(n) mod 2.
 * https://oeis.org/A230298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A010062(n) mod 2, where A010062(0)=1, A010062(k+1) =
 *            A010062(k) + popcount(A010062(k))
 * Range:     u8 n <= 71, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A010062_pop(seq_t x) {
    seq_t c = 0;
    while (x != 0) {
        c += (x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t x = 1;
    for (idx_t i = 0; i < n; ++i) {
        x = x + A010062_pop(x);
    }
    return x & 1;
}
