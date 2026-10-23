/*
 * A135546  Let p be the n-th prime and let g be the order of 2 mod p (see
 *          A014664). Then if g is even, a(n) = p*(2^(g/2) - 1), otherwise
 *          a(n) = 2^g - 1.
 * https://oeis.org/A135546
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined as follows. Let p be the n‑th prime (indexed
 *            from 1, the offset of the sequence is 2). Let g be the
 *            multiplicative order of 2 modulo p (i.e. the smallest positive
 *            integer such that 2^g ≡ 1 (mod p)). If g is even, a(n) = p •
 *            (2^(g/2) – 1); otherwise a(n) = 2^g – 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* out of defined range */

    /* prime p = n‑th prime */
    uint32_t p32 = get_prime(n);
    seq_t   p   = (seq_t)p32;

    /* initial order candidate = p-1 (groups of prime modulus) */
    idx_t  ord   = (idx_t)p32 - 1;
    idx_t  order = ord;           /* will be reduced to the true order */
    idx_t  rem   = ord;           /* used for prime factorisation */

    /* factorise rem and reduce order for each prime factor */
    for (idx_t i = 2; i * i <= rem; ++i) {
        if (rem % i == 0) {
            /* reduce order by this factor as much as possible */
            while (order % i == 0 &&
                   mod_pow((seq_t)2, order / i, p) == (seq_t)1)
                order /= i;

            /* remove factor from rem completely */
            while (rem % i == 0) rem /= i;
        }
    }
    if (rem > 1) {                 /* remaining prime factor */
        while (order % rem == 0 &&
               mod_pow((seq_t)2, order / rem, p) == (seq_t)1)
            order /= rem;
    }

    /* compute the term according to the parity of the order */
    if ((order & 1ULL) == 0) {     /* even order */
        seq_t half = pow2(order / 2);          /* 2^(g/2) */
        return p * (half - (seq_t)1);
    } else {                       /* odd order */
        seq_t full = pow2(order);               /* 2^g */
        return full - (seq_t)1;
    }
}
