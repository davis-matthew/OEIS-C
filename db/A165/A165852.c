/*
 * A165852  Totally multiplicative sequence with a(p) = 31.
 * https://oeis.org/A165852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 31^Ω(n) where Ω(n) is the total number of prime factors
 *            of n (counted with multiplicity)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9316 (2026-11-01)
 * Helpers:   bigomega, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;          /* undefined for n < 1, guard against accidental calls */
    }
    seq_t omega = bigomega(n);    /* Ω(n) */
    return ipow((seq_t)31, omega);  /* 31^Ω(n) */
}
