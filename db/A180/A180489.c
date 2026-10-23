/*
 * A180489  Smallest pandigital number (A171102) divisible by the n-th prime
 *          A000040(n).
 * https://oeis.org/A180489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: a(n) is the smallest pandigital number
 * (containing every decimal digit 0‑9 at least once) that is
 * divisible by the nth prime.
 */
#include <stdint.h>
#include "../utils/get_prime.h"

/* Test whether x contains all decimal digits at least once. */
static inline int is_pandigital(seq_t x)
{
    unsigned char cnt[10];
    for (int i = 0; i < 10; ++i) cnt[i] = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        cnt[d]++;
        x /= 10;
    }
    for (int i = 0; i < 10; ++i) {
        if (cnt[i] == 0) return 0;
    }
    return 1;
}

/* Return the smallest pandigital multiple of the nth prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */
    const seq_t START = 1023456789;           /* smallest pandigital number */
    uint32_t prime = get_prime(n);            /* nth prime (uint32_t OK) */

    /* Start from the first multiple of prime that is >= START */
    seq_t k = START / (seq_t)prime;
    if ((seq_t)prime * k < START) ++k;

    for (;;) {
        seq_t cand = (seq_t)prime * k;
        if (is_pandigital(cand)) return cand;
        ++k;
    }
}
