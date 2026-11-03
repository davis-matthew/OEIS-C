/*
 * A005279  Numbers having divisors d, e with d < e < 2*d.
 * https://oeis.org/A005279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers with divisors d, e such that d < e < 2*d
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8530 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t collect_divisors(seq_t m, seq_t *divs, idx_t max_divs) {
    idx_t count = 0;
    for (seq_t d = 1; d * d <= m && count < max_divs; d++) {
        if (m % d == 0) {
            divs[count++] = d;
            if (d != m/d && count < max_divs) {
                divs[count++] = m / d;
            }
        }
    }
    return count;
}

static bool has_divisor_pair(seq_t m) {
    seq_t divs[512];
    idx_t count = collect_divisors(m, divs, 512);
    
    for (idx_t i = 0; i < count; i++) {
        for (idx_t j = 0; j < count; j++) {
            seq_t d = divs[i];
            seq_t e = divs[j];
            if (d < e && e < 2*d) {
                return true;
            }
        }
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    
    while (count < n) {
        m++;
        if (has_divisor_pair(m)) {
            count++;
        }
    }
    
    return m;
}
