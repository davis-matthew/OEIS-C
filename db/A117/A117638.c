/*
 * A117638  Numbers k for which the digital root of phi(k) divides k.
 * https://oeis.org/A117638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate integers k, keep those for which the digital root
 * of Euler's totient φ(k) divides k. Return the n‑th such integer. */
#include <stdbool.h>

/* Compute φ(n) for a 64‑bit or wider unsigned integer.
 * Uses trial division with incremental trial factors.
 */
static seq_t phi_func(seq_t n)
{
    seq_t result = n;
    seq_t temp   = n;
    for (seq_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 as safe default */
        return 0;

    seq_t    k     = 1;     /* current integer to test */
    idx_t    count = 0;     /* how many qualifying integers found */

    while (1) {
        seq_t ph = phi_func(k);                       /* φ(k) */
        seq_t dr = 1 + (ph - 1) % 9;                  /* digital root of φ(k) */
        if (k % dr == 0) {                            /* φ(k) dr divides k? */
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
