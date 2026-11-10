/*
 * A076054  a(n) = Sum_{k=1..n} A006513(k).
 * https://oeis.org/A076054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A006513(k), where A006513(k) = 1 if the
 *            (3x+1)/2 map takes an even number of steps to reach 1 from k,
 *            else 2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A006513_parity_odd(idx_t k) {
    unsigned __int128 x = (unsigned __int128)k;
    unsigned long long count = 0;
    while (x != 1) {
        if (x & 1) {
            x = (3 * x + 1) / 2;
        } else {
            x = x / 2;
        }
        count++;
    }
    return (int)(count & 1);
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 1; k <= n; k++) {
        total += (seq_t)(A006513_parity_odd(k) ? 2 : 1);
    }
    return total;
}
