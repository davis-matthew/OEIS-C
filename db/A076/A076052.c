/*
 * A076052  Sum(k=1, n, A006460(k)).
 * https://oeis.org/A076052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A006460(k), where A006460(k) is the image of
 *            k after 3j Collatz steps (j large)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9987 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A006460_val(uint64_t n) {
    static const unsigned values[3] = {1, 4, 2};
    unsigned __int128 x = n;
    int step_mod3 = 0;
    while (x != 1 && x != 2 && x != 4) {
        if ((x & 1) == 0) x >>= 1;
        else x = 3 * x + 1;
        step_mod3 = (step_mod3 + 1) % 3;
    }
    int offset_x = (x == 1) ? 0 : (x == 4) ? 1 : 2;
    int final_offset = (offset_x - step_mod3 + 3) % 3;
    return values[final_offset];
}

seq_t a(idx_t n) {
    seq_t s = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; k++) {
        s += (seq_t)A006460_val(k);
    }
    return s;
}
