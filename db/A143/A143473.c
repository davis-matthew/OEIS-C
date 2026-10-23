/*
 * A143473  Replace the leading digit d of n with 10-d (in decimal
 *          representation).
 * https://oeis.org/A143473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (10 - d) * 10^k + r where d = the most significant
 *            decimal digit of n, k = floor(log10(n)) (i.e., the power of 10
 *            such that 10^k ≤ n < 10^(k+1)), r = n mod 10^k (the remaining
 *            digits after the first one)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the largest power of 10 not exceeding n.  p = 10^k. */
    idx_t p = 1;
    while (n / 10 >= p)
        p *= 10;

    /* Extract the most significant digit d and the remaining part r. */
    idx_t d   = n / p;     /* 0 ≤ d ≤ 9, d = leading digit (d = 0 only for n = 0) */
    idx_t r   = n % p;     /* the trailing digits after the leading one     */

    /* Compute the transformed number. */
    seq_t new_lead = (seq_t)(10 - d);   /* replace by 10-d */
    seq_t result   = new_lead * (seq_t)p + (seq_t)r;
    return result;
}
