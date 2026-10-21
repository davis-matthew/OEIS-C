/*
 * A056852  a(n) = (p^p + 1)/(p + 1), where p = prime(n).
 * https://oeis.org/A056852
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^p + 1)/(p + 1) where p = prime(n) (offset 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 2, behaviour for n < 2 is irrelevant */
    if (n < 2)
        return (seq_t)0;

    /* obtain the n‑th prime (1‑based) */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^p + 1 */
    seq_t val = big_pow(p, p);
    val += (seq_t)1;

    /* divide by p + 1 (division is guaranteed to be exact) */
    return val / (p + (seq_t)1);
}
