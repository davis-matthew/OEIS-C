/*
 * A104127  a(n) = (1+prime(n))^prime(n).
 * https://oeis.org/A104127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + prime(n))^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1; raise (1 + prime(n)) to prime(n) */
    seq_t p   = get_prime((uint64_t)n); /* nth prime */
    seq_t base = p + (seq_t)1;
    idx_t  exp  = (idx_t)p;
    return seq_pow(base, exp);
}
