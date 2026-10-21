/*
 * A057062  Let R(i,j) be the infinite square array with antidiagonals 1; 2,3;
 *          4,5,6; ...; the n-th prime is in antidiagonal a(n).
 * https://oeis.org/A057062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((sqrt(1 + 8*(prime(n)-1)) + 1) / 2)
 * Range:     u8 n <= 4, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently: a(n) = round(sqrt(2*prime(n)))).           */

#include "../utils/floor_sqrt.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    uint32_t p = get_prime(n);                      /* nth prime (uint32_t)   */
    seq_t val = ((seq_t)8 * (seq_t)p) - (seq_t)7;    /* 1 + 8*(p-1) = 8p-7      */
    seq_t s   = floor_sqrt(val);                    /* floor( sqrt(val) )     */
    seq_t res = (s + (seq_t)1) / (seq_t)2;          /* (s+1)/2 as floor       */
    return res;
}
