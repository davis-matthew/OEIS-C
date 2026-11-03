/*
 * A165831  Totally multiplicative sequence with a(p) = 10.
 * https://oeis.org/A165831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 10^A001222(n) = 10^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9751 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* bigomega(n) gives the total number of prime factors of n
       (counted with multiplicity).  pow10(e) returns 10^e
       as a seq_t value. */
    return pow10((idx_t)bigomega(n));
}
