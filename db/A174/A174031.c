/*
 * A174031  The smallest integer k>0 such that the double-concatenation
 *          prime(n) // prime(n+1) // k is a prime number.
 * https://oeis.org/A174031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   concat_parts, digit_len, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A174031.
   For a given index n (1‑based) it finds the smallest integer k > 0
   such that the concatenation of prime(n), prime(n+1) and k forms a
   prime number.  Leading zeros in k are not allowed.  The search
   skips even numbers and numbers ending in 5, since the result must
   have last decimal digit 1,3,7 or 9. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; get the nth and (n+1)th primes. */
    idx_t p1 = get_prime(n);
    idx_t p2 = get_prime(n + 1);

    /* Concatenate prime(n) and prime(n+1).  concat_parts takes idx_t
       arguments so we use the values directly. */
    seq_t base = concat_parts(p1, p2);

    /* Search for the smallest k. */
    uint64_t k = 1;
    for (;; ++k) {
        /* k must be odd and not end with 5. */
        uint8_t ld = k % 10;
        if ((ld & 1) == 0 || ld == 5)
            continue;

        seq_t kk = (seq_t)k;
        int len = digit_len(kk);          /* number of decimal digits of k */
        seq_t pow10k = pow10((idx_t)len); /* 10^len */
        seq_t candidate = base * pow10k + kk;

        if (is_prime_big(candidate))
            return kk;
    }
}
