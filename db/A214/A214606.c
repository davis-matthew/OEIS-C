/*
 * A214606  a(n) = gcd(n, 2^n - 2).
 * https://oeis.org/A214606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, 2^n - 2)
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (the offset of the sequence) */
    seq_t n_seq = (seq_t)n;                 /* cast to the seq_t type   */
    seq_t t      = mod_pow((seq_t)2, n, n_seq);   /* 2^n mod n                */
    /* Compute (t - 2) modulo n, taking care of negative values for signed types */
    seq_t temp = (t + n_seq - (seq_t)2) % n_seq;
    if (temp < (seq_t)0)                   /* adjust negative remainder */
        temp += n_seq;
    /* Greatest common divisor of n and (2^n - 2) */
    return gcd(n_seq, temp);
}
