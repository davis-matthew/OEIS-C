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
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"  /* provides unsigned floor_log2_uint64(idx_t) */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;           /* start with n itself        */
    idx_t  term   = n;                 /* current value to log       */
    unsigned logv = floor_log2_uint64(term);  /* floor(log₂(term))       */

    while (logv > 1) {                 /* iterate while log₂ ≥ 2     */
        result *= (seq_t)logv;         /* multiply by current log    */
        term   = logv;                 /* next term is this log      */
        logv   = floor_log2_uint64(term);          /* next log                */
    }
    return result;
}
