/*
 * A007546  Number of steps to compute n-th prime in PRIMEGAME (fast version).
 * https://oeis.org/A007546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Sum_{m=2..prime(n)} c(m), where c(m) is the exact number
 *            of PRIMEGAME steps spent on m. This comes from reading the
 *            program's loops as a register machine: for each m it runs 3m-1
 *            setup steps, then tests d = m-1, m-2, ... down to the largest
 *            proper divisor D of m (D = 1 when m is prime). Wi...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 233, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   cost_of, floor_sum, spf_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cost_of.h"
#include "../utils/floor_sum.h"
#include "../utils/spf_u64.h"

seq_t a(idx_t n) {
    static idx_t cnt = 0;
    static uint64_t M = 1;
    static seq_t total = 0;

    if (n < cnt) {
        cnt = 0;
        M = 1;
        total = 0;
    }
    while (cnt < n) {
        M++;
        uint64_t p = spf_u64(M);
        total += (seq_t)cost_of(M, p);
        if (p == M) cnt++;
    }
    return total;
}
