/*
 * A216761  n * floor(log_2(n)) * floor(log_2(log_2(n))) *
 *          floor(log_2(log_2(log_2(n)))) ....
 * https://oeis.org/A216761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n · ⎣log₂(n)⎦ · ⎣log₂(⎣log₂(n)⎦)⎦ ·
 *            ⎣log₂(⎣log₂(⎣log₂(n)⎦)⎦)⎦ · … product continues while the
 *            argument to log₂ remains > 1
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;           /* start with n itself        */
    idx_t  term   = n;                 /* current value to log       */
    unsigned logv = ilog2(term);  /* floor(log₂(term))       */

    while (logv > 1) {                 /* iterate while log₂ ≥ 2     */
        result *= (seq_t)logv;         /* multiply by current log    */
        term   = logv;                 /* next term is this log      */
        logv   = ilog2(term);          /* next log                */
    }
    return result;
}
