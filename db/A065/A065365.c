/*
 * A065365  Replace 3^k with 2^k in balanced ternary expansion of n.
 * https://oeis.org/A065365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ d_k·2^k where d_k are the balanced ternary digits of n.
 *            (Compute the balanced ternary digits of n, then evaluate the
 *            same signed expansion with powers of 2 instead of 3.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pos accumulates contributions from +1 digits,
       neg accumulates contributions from -1 digits. */
    seq_t pos = 0;
    seq_t neg = 0;
    seq_t pow = 1;          /* current power of 2 (2^k) */
    uint64_t t = n;         /* work with unsigned 64‑bit value */

    while (t > 0) {
        uint64_t r = t % 3;
        t /= 3;

        if (r == 2) {              /* digit = -1 */
            neg += pow;            /* subtract 2^k later */
            ++t;                   /* carry to next digit (3^(k+1)) */
        } else if (r == 1) {       /* digit = +1 */
            pos += pow;            /* add 2^k */
        }
        pow *= 2;                  /* next power of 2 */
    }
    /* pos is guaranteed to be ≥ neg for positive n, so unsigned subtraction
       yields the correct result. */
    return pos - neg;
}
