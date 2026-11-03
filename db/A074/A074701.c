/*
 * A074701  Numbers k such that k = Sum_{d|phi(k)} mu(phi(d))*phi(k)/d.
 * https://oeis.org/A074701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A074701
 * Formula: 
 *   a(1) = 1
 *   a(2) = 3
 *   a(n) = 5^(n-2)      for n ≥ 3
 * This follows from the fact that the sequence consists of 1, 3 and all powers of 5.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Compute 5^(n-2) using big exponentiation */
    seq_t base = (seq_t)5;
    seq_t exp  = (seq_t)(n - 2);
    return ipow(base, exp);
}
