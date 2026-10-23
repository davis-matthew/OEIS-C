/*
 * A161344  Numbers k with A033676(k)=2, where A033676 is the largest divisor
 *          <= sqrt(k).
 * https://oeis.org/A161344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(1) = 4, a(2) = 6, a(3) = 8; * a(n) = 2 * p_{n-1} for n ≥ 4,
 *            where p_k denotes the k-th prime. * This follows from OEIS
 *            A161344 = 2 * A000040 ∪ {8}
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the first three special cases explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)8;

    /* For n ≥ 4: return twice the (n−1)-st prime */
    /* get_prime returns a 32‑bit prime; cast to seq_t for generality */
    uint32_t p = get_prime(n - 1);
    return (seq_t)2 * (seq_t)p;
}
