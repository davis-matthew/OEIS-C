/*
 * A180560  The number of times the n-th prime, p, can become a different
 *          prime under any mapping of some single decimal digit <=> with some
 *          other single decimal digit.
 * https://oeis.org/A180560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1377, u32 n <= 1362, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all digit swaps on the n‑th prime. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t) -> uint32_t */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) -> bool */

/* Return the sequence value a(n).  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n < 1, safe return */
        return 0;

    /* Obtain the n-th prime (1‑based).  The helper returns a 32‑bit value. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Extract decimal digits of p, least significant first. */
    uint8_t rev_digits[64];
    size_t dlen = 0;
    seq_t tmp = p;
    while (tmp > 0) {
        rev_digits[dlen++] = (uint8_t)(tmp % 10);
        tmp = tmp / 10;
    }

    /* digits array in most significant first order. */
    uint8_t digits[64];
    for (size_t i = 0; i < dlen; ++i)
        digits[i] = rev_digits[dlen - 1 - i];

    /* Array to store distinct primes obtained, max 45 pairs. */
    seq_t uniq[45];
    size_t uniq_cnt = 0;

    /* Enumerate all unordered digit pairs (a,b), a<b. */
    for (int a = 0; a < 9; ++a) {
        for (int b = a + 1; b < 10; ++b) {
            seq_t new_val = 0;

            /* Build the new number by applying the swap. */
            for (size_t i = 0; i < dlen; ++i) {
                uint8_t dg = digits[i];
                if (dg == a)
                    dg = b;
                else if (dg == b)
                    dg = a;
                new_val = new_val * 10 + (seq_t)dg;
            }

            if (new_val == p || new_val == 0)
                continue;              /* no change or zero */

            if (!is_prime_big(new_val))
                continue;              /* not prime */

            /* Check for duplicates. */
            bool dup = false;
            for (size_t j = 0; j < uniq_cnt; ++j) {
                if (uniq[j] == new_val) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                uniq[uniq_cnt++] = new_val;
                if (uniq_cnt >= 45)      /* safety, though impossible */
                    break;
            }
        }
        if (uniq_cnt >= 45)
            break;
    }

    return (seq_t)uniq_cnt;
}
