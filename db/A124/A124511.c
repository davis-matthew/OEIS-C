/*
 * A124511  a(n) = A124508(A124508(n)).
 * https://oeis.org/A124511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * 2^(ω(n)+Ω(n)), a(1)=1
 * Range:     u8 n <= 11, u16 n <= 3071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ω(n) = number of distinct prime factors of n */
/* Ω(n) = total number of prime factors of n, counted with multiplicity */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;            /* Offset: first element is 1 */

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Compute ω(n) and Ω(n) */
    size_t omega = cnt;
    size_t omega_total = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega_total += exps[i];

    int exp = (int)(omega + omega_total);   /* exponent of 2 */

    /* a(n) = 9 * 2^exp */
    seq_t val = seq_pow((seq_t)2, (idx_t)exp);
    return (seq_t)9 * val;
}
