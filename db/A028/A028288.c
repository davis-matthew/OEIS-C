/*
 * A028288  Molien series for complex 4-dimensional Clifford group of order
 *          92160 and genus 2. Also Molien series of ring of biweight
 *          enumerators of Type II self-dual binary codes.
 * https://oeis.org/A028288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((n^3 + 12*n^2 + 49*n + 135 + 5*n*((n+1)mod3 -
 *            nmod3))/135)
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n ≥ 0, with n counted starting from 0 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t ns = (seq_t)n;
    /* n^2 and n^3 */
    seq_t nsq = ns * ns;
    seq_t ncu = nsq * ns;

    /* base = n^3 + 12*n^2 + 49*n + 135 */
    seq_t base = ncu;
    base += (seq_t)12 * nsq;
    base += (seq_t)49 * ns;
    base += (seq_t)135;

    /* diff = ((n+1) mod 3) - (n mod 3)  (fits in a signed int) */
    int diff = (int)((n + 1) % 3) - (int)(n % 3);

    /* add 5*n*diff safely (diff can be negative) */
    if (diff >= 0) {
        base += (seq_t)(5 * diff) * ns;
    } else {
        base -= (seq_t)(5 * (-diff)) * ns;
    }

    /* final division by 135 (sequence value fits in seq_t) */
    return base / (seq_t)135;
}
