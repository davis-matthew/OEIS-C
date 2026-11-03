/*
 * A219029  a(n) = n - 1 - phi(phi(n)).
 * https://oeis.org/A219029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - 1 - phi(phi(n))
 * Range:     i8 n <= 132, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 972 (2026-11-01)
 * Helpers:   totient, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/totient_seq.h"

/* seq_t a(idx_t n) computes OEIS sequence A219029: a(n) = n - 1 - phi(phi(n)). */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t phi_n = totient(n);          /* phi(n) */
    seq_t phi_phi_n = totient_seq(phi_n);  /* phi(phi(n)) */
    return nseq - 1 - phi_phi_n;
}
