/*
 * A176597  Double primes: concatenation of the n-th prime with itself.
 * https://oeis.org/A176597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(p_n, p_n)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* n-th prime (1‑indexed) */
    int d      = digits(p);            /* number of decimal digits of p  */
    seq_t pow  = pow10(d);              /* 10^d  */
    seq_t p_seq = (seq_t)p;             /* convert p to seq_t */
    return p_seq * pow + p_seq;         /* concatenate p with itself */
}
