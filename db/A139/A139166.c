/*
 * A139166  a(n) = (prime(n)!+8)/8.
 * https://oeis.org/A139166
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)! + 8) / 8
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   fact_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fact_upto.h"

/* helper to convert an integer literal to seq_t in a way that works for both C and C++ */
#if defined(__cplusplus)
#define TO_SEQ_T(v) seq_t(v)
#else
#define TO_SEQ_T(v) (seq_t)(v)
#endif

seq_t a(idx_t n)
{
    /* prime(n) is an unsigned integer (uint32_t); convert to idx_t for the factorial routine */
    idx_t p = get_prime(n);            /* p = prime(n)   */
    seq_t f = fact_upto(p);            /* f = p!         */
    return (f + TO_SEQ_T(8)) / TO_SEQ_T(8);  /* (p! + 8) / 8   */
}
