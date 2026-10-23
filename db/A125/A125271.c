/*
 * A125271  Number of Gaussian integer divisors of n (having positive real
 *          part).
 * https://oeis.org/A125271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A062327(n) - A000005(n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of Gaussian integer divisors of n
   having positive real part. */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    /* exponent of 2 */
    idx_t exp2 = 0;
    while ((tmp & 1U) == 0 && tmp != 0) {
        exp2++;
        tmp >>= 1;
    }

    /* Gaussian divisor count up to associates (A062327) */
    seq_t gaussian = (seq_t)(2 * exp2 + 1);
    /* Integer divisor count (A000005) */
    seq_t divisor = (seq_t)(exp2 + 1);

    /* factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                e++;
                tmp /= p;
            }
            divisor *= (seq_t)(e + 1);

            if ((p & 3U) == 1) {          /* p ≡ 1 (mod 4) */
                seq_t f = (seq_t)(e + 1);
                gaussian *= f;
                gaussian *= f;
            } else { /* p ≡ 3 (mod 4) */
                gaussian *= (seq_t)(e + 1);
            }
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        divisor *= 2;       /* exponent is 1 */
        if ((tmp & 3U) == 1) {   /* p ≡ 1 (mod 4) */
            gaussian *= 4;
        } else {                 /* p ≡ 3 (mod 4) */
            gaussian *= 2;
        }
    }

    return gaussian * (seq_t)2 - divisor;
}
