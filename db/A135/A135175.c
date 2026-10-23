/*
 * A135175  a(n) = 5^p + 3^p - 2^p, where p = prime(n).
 * https://oeis.org/A135175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^p + 3^p - 2^p where p = A000040(n) (the n‑th prime)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/get_prime.h" /* uint32_t get_prime(idx_t k); */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* defined only for n ≥ 1; return 0 for safety */

    /* Retrieve the n‑th prime (1‑based index). */
    seq_t prime = (seq_t)get_prime(n);

    /* Compute 5^prime + 3^prime - 2^prime using big_pow. */
    seq_t five_exp  = big_pow((seq_t)5, prime);
    seq_t three_exp = big_pow((seq_t)3, prime);
    seq_t two_exp   = big_pow((seq_t)2, prime);

    return five_exp + three_exp - two_exp;
}
