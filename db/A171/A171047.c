/*
 * A171047  In the sequence of prime numbers, replace all the '8' digits with
 *          '6' and vice versa.
 * https://oeis.org/A171047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digits 6 ↔ 8 of the n‑th prime (1‑based)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/*------------------------------------------------------------------*/
/* Swap digits 6 and 8 in the decimal representation of v.        */
/*------------------------------------------------------------------*/
static seq_t
swap_digits(seq_t v)
{
    if (v == 0) return 0;

    /* store digits LSB→MSB in a small fixed buffer */
    uint8_t buf[40];           /* 128‑bit seq_t has < 39 decimal digits */
    idx_t   len = 0;
    seq_t   t   = v;

    while (t > 0) {
        seq_t d = t % 10;
        t /= 10;
        if (d == 6)
            d = 8;
        else if (d == 8)
            d = 6;
        buf[len++] = (uint8_t)d;
    }

    /* rebuild number in correct order */
    seq_t res = 0;
    for (idx_t i = len; i > 0; --i)
        res = res * 10 + buf[i - 1];

    return res;
}

/*------------------------------------------------------------------*/
/* Find the n-th prime (1‑based index).                            */
/*------------------------------------------------------------------*/
static seq_t
nth_prime(idx_t n)
{
    idx_t count   = 0;
    idx_t cand    = 2;          /* candidate tested for primality */

    while (1) {
        if (is_prime(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}

/*------------------------------------------------------------------*/
/* Public sequence function.                                       */
/*------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe return */

    seq_t prime = nth_prime(n);      /* obtain the n‑th prime */
    return swap_digits(prime);       /* swap 6↔8 in its decimal digits */
}
