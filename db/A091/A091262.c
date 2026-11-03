/*
 * A091262  Sum of totient function values of powers of n, as exponent runs
 *          from 1 to n.
 * https://oeis.org/A091262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^n-1)/(n-1))*phi(n), n>1; a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t sn   = (seq_t)n;          /* base = n */
    seq_t pn   = ipow(sn, sn);   /* n^n */
    seq_t num  = pn - (seq_t)1;     /* n^n - 1 */
    seq_t den  = sn - (seq_t)1;     /* n - 1 */
    seq_t quotient = num / den;    /* (n^n-1)/(n-1) */
    seq_t phival = totient(n);          /* phi(n) */
    return quotient * phival;
}
