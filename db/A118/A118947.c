/*
 * A118947  n-th (starting from the left) decimal digit of 13^n.
 * https://oeis.org/A118947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 70, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   mod_pow, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A118947:
 *  a(n) = the n-th decimal digit (counting from the left) of 13^n.
 *  The value is computed as:
 *       k  = floor( n * (log10(13) – 1) ) + 1
 *       t  = 13^n  (mod 10^(k+1))
 *       d  = (t / 10^k) % 10
 *  where k = number of digits of 13^n minus n.
 */
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    /* log10(13) constant */
    const double LOG10_13 = 1.113943352306122234;  /* = log10(13) */

    /* d = number of decimal digits of 13^n = floor(n*log10(13)) + 1 */
    double ln = (double)n * LOG10_13;
    uint64_t d = (uint64_t)ln + 1;     /* floor(ln) + 1 */

    /* k = d – n  →  number of trailing digits that are removed */
    uint64_t k = d - n;

    /* modulus 10^(k+1) and 10^k */
    seq_t mod     = pow10(k + 1);  /* 10^(k+1) */
    seq_t pow10k  = pow10(k);      /* 10^k */

    /* 13^n modulo 10^(k+1) */
    seq_t r = mod_pow((seq_t)13, n, mod);

    /* Extract the desired digit */
    seq_t q = r / pow10k;      /* floor(13^n / 10^k)  */
    seq_t digit = q % (seq_t)10;   /* take the last digit */

    return digit;
}
