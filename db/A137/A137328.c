/*
 * A137328  a(n) = prime(n) - primorial(k), where k is the greatest number for
 *          which primorial(k) <= prime(n).
 * https://oeis.org/A137328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) - primorial(k), where k = the greatest integer
 *            such that primorial(k) <= prime(n)
 * Range:     u8 n <= 54, u16 n <= 7607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  Sequence A137328 – difference between the nth prime and the largest
    primorial not exceeding it.  Computes directly without pre‑saving
    tables, works for any supported seq_t type and any idx_t input.     */
/* --------------------------------------------------------------------- */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n < 1) return 0;

    /* Prime at position n. */
    seq_t prime_n = (seq_t)get_prime(n);

    /* Compute primorial(k) iteratively. */
    seq_t primorial = 1;        /* primorial(0) */
    idx_t  k = 0;               /* current exponent */

    for (;;) {
        idx_t next_k = k + 1;
        seq_t next_prime = (seq_t)get_prime(next_k);

        /* If multiplying would exceed prime_n, stop.
           Avoid overflow by using division instead of multiplication. */
        if (prime_n / primorial < next_prime) {
            break;                 /* primorial(k) is now the largest <= prime_n */
        }

        primorial = primorial * next_prime;
        k = next_k;
    }

    /* Return the difference. */
    return prime_n - primorial;
}
