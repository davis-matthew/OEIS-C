/*
 * A105331  Numbers of the form 2^n*(2^(n+1)+2n+1) where 2^(n+1)+2n+1 is
 *          prime.
 * https://oeis.org/A105331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = 2^k · (2^(k+1)+2k+1) where 2^(k+1)+2k+1 is prime. k is
 *            the nth integer (starting from 0) for which the innermost
 *            quantity is prime. The function returns the nth term of this
 *            sequence. It relies on the helper is_prime() for 64‑bit numbers
 *            and pow2() for computing 2^k. The implementation sto...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index of the sequence.  We count primes in
       increasing order of k (starting from 0) until we reach the
       nth term. */
    idx_t count = 0;          /* number of terms found so far */

    /* We only need to examine k up to 63, because at k = 63 the
       inner quantity 2^(k+1)+2k+1 exceeds 2^64−1 and will not
       fit in a 64‑bit integer.  Once it exceeds this bound, all
       later inner quantities also exceed the bound, so the
       sequence of 64‑bit values ends. */
    for (idx_t k = 0; k <= 63; ++k) {
        /* Compute p = 2^(k+1) + 2*k + 1 using 128‑bit arithmetic
           to avoid overflow. */
        __uint128_t pow2_val = ((unsigned __int128)1) << (k + 1);
        __uint128_t p = pow2_val + (__uint128_t)(2 * k) + 1;

        /* Check if p fits in 64 bits; otherwise we cannot test
           primality with the available helper and we terminate. */
        if (p > (unsigned __int128)UINT64_MAX) {
            break;   /* no further 64‑bit terms exist */
        }

        /* Test primality of p.  is_prime() works for 64‑bit inputs. */
        if (is_prime((uint64_t)p)) {
            ++count;
            if (count == n) {
                /* Compute 2^k * p, using pow2() to get a(seq_t). */
                seq_t term = pow2(k) * (seq_t)p;
                return term;
            }
        }
    }

    /* If we reach here, the requested term does not fit in a 64‑bit
       seq_t or was beyond the already known 64‑bit terms.  Return
       zero; the calling framework will not rely on this for
       64‑bit builds. */
    return (seq_t)0;
}
