/*
 * A007424  a(n) = 1 if n is squarefree, otherwise 2.
 * https://oeis.org/A007424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is squarefree, otherwise 2
 * Range:     u8 n <= 9776, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree(uint64_t n) {
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                count++;
                n /= i;
                if (count > 1) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n) {
    return is_squarefree((uint64_t)n) ? (seq_t)1 : (seq_t)2;
}
