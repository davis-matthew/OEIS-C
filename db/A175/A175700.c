/*
 * A175700  a(n) = n ^ phi(n+1), phi(n) = A000010(n) = Euler totient function.
 * https://oeis.org/A175700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n ^ φ(n+1) (Euler totient)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 287 (2026-11-01)
 * Helpers:   ipow, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset */
    seq_t base = (seq_t)n;
    seq_t exp  = totient_seq((seq_t)(n + 1));
    return ipow(base, exp);
}
