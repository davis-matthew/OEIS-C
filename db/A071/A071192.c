/*
 * A071192  Least m>n such that the number of prime factors of m and n differ
 *          by 1.
 * https://oeis.org/A071192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > n such that |Ω(m) - Ω(n)| = 1 where Ω(k) is the
 *            total number of prime factors of k counted with multiplicity
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega_value.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    /* OEIS definition starts at n = 1 */
    if (n == 0) return 0;          /* for safety, though n>=1 in practice */

    uint32_t om_n = omega_value(n);

    idx_t m = n + 1;
    for (;; ++m) {
        /* stop on integer wrap */
        if (m == 0) return 0;
        uint32_t om_m = omega_value(m);
        if (om_m == om_n + 1 || om_m + 1 == om_n) {
            return (seq_t)m;
        }
    }
}
