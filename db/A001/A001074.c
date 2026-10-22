/*
 * A001074  Numbers m such that Sum_{k=0..m-1} exp(2*Pi*i*k^3/m) != 0.
 * https://oeis.org/A001074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   search over m. The cubic Gauss sum is multiplicative in m (up to
 *            unit coefficients), and for each p^e || m it is zero iff e == 1
 *            (mod 3) and p != 1 (mod 3). So m is a term iff no prime p with p
 *            == 0 or 2 (mod 3) divides m to a power e == 1 (mod 3)
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001074_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001074_ok.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    while (count < n) {
        m++;
        if (a001074_ok(m)) count++;
    }
    return (seq_t)m;
}
