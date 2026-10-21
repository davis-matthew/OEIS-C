/*
 * A072779  a(n) = sigma_2(n) + phi(n) * sigma(n).
 * https://oeis.org/A072779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = sigma_2(n) + phi(n) * sigma(n)
 * Range:     u8 n <= 11, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where sigma(n)  = sum of divisors of n            */
/*                sigma_2(n)= sum of squares of divisors of n*/
/*                phi(n)   = Euler's totient function          */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/sigma_k.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, guard against unwanted 0 index */
        return (seq_t)0;

    seq_t sigma  = sigma_of(n);          /* sigma(n) */
    seq_t sigma2 = sigma_k(n, (uint8_t)2);/* sigma_2(n) */
    seq_t phi    = calc_phi(n);          /* phi(n) */

    return sigma2 + phi * sigma;
}
