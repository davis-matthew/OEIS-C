/*
 * A138822  Concatenation of n-th prime and n-th Fibonacci number.
 * https://oeis.org/A138822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the n-th prime and the n-th Fibonacci
 *            number
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 78, u128 n <= 169,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence requires the n-th prime (A000040) and the n-th Fibonacci number
   (A000045).  We compute the concatenation by multiplying the prime by 10^k
   where k is the number of decimal digits of the Fibonacci number, and then
   adding the Fibonacci number.  All arithmetic uses the seq_t type supplied
   by the build. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci value at index n */
    seq_t f = fib(n);

    /* Number of decimal digits of f (digit_len utility not used to avoid
       additional header dependencies) */
    seq_t tmp = f;
    int digits = 0;
    do {
        digits++;
        tmp /= 10;
    } while (tmp != 0);

    /* 10^digits as seq_t */
    seq_t tenpow = 1;
    for (int i = 0; i < digits; ++i) {
        tenpow *= 10;
    }

    /* n-th prime value */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Concatenation: prime * 10^digits + fib */
    return p * tenpow + f;
}
