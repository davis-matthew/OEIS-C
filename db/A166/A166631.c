/*
 * A166631  Totally multiplicative sequence with a(p) = 10p for prime p.
 * https://oeis.org/A166631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 10^Ω(n) (Ω = total number of prime factors with
 *            multiplicity)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9974 (2026-11-01)
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

/* Compute the OEIS sequence A166631:
 * a(1) = 1
 * a(n) = n * 10^bigomega(n) for n >= 1
 */
seq_t a(idx_t n)
{
    /* bigomega returns the number of prime factors counted with multiplicity.
       For n >= 1 it is a non‑negative integer that easily fits in idx_t.
    */
    seq_t omega_seq = bigomega(n);
    idx_t exp = (idx_t)omega_seq;   /* conversion safe for all practical n */

    seq_t pow = pow10(exp);          /* 10^exp */
    seq_t res = ((seq_t)n) * pow;    /* n * 10^Ω(n) */

    return res;
}
