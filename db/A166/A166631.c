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
 * Helpers:   bigomega, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/pow10.h"

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
