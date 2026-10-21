/*
 * A088264  Smallest number k > 0 such that prefixing k to the n-th quadruple
 *          in the set {(1,3,7,9), (11,13,17,19), (21,23,27,29), ...} yields
 *          all primes.
 * https://oeis.org/A088264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_uint64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088264
 *  a(n) = smallest k > 0 such that the numbers
 *          k*10^d + (10·(n−1)+1, 10·(n−1)+3, 10·(n−1)+7, 10·(n−1)+9)
 *  are all prime, where d = digits(10·(n−1)+1).
 *
 *  The implementation searches k sequentially, testing the four
 *  candidates for primality.  For values that fit in 64 bits the
 *  deterministic Miller‑Rabin test is used; for larger values the
 *  arbitrary‑precision test provided by the utilities is invoked.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

/* number of decimal digits of an unsigned 64‑bit integer (x > 0) */
static inline int digits_uint64(uint64_t x)
{
    int d = 1;
    while (x >= 10ULL) { x /= 10ULL; ++d; }
    return d;
}

/* primality test for seq_t values: use 64‑bit test if possible,
 * otherwise call the arbitrary‑precision routine. */
static inline bool is_prime_any(seq_t n)
{
    if (n <= (seq_t)UINT64_MAX)
        return is_prime_uint64((uint64_t)n);
    return is_prime_big(n);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n=0 */

    /* base = 10·(n−1) as uint64_t */
    uint64_t base = 10ULL * ((uint64_t)n - 1ULL);

    /* number of digits of the quadruple members */
    int d = digits_uint64(base + 1ULL);

    /* factor = 10^d as seq_t */
    seq_t factor = pow10((idx_t)d);

    /* offsets of the four members of the quadruple */
    const int ofs[4] = {1, 3, 7, 9};

    seq_t k = 1;                          /* candidate k > 0 */
    for (;;) {
        bool all_prime = true;
        for (int i = 0; i < 4; ++i) {
            seq_t val = factor * k + (seq_t)(base + (uint64_t)ofs[i]);
            if (!is_prime_any(val)) {
                all_prime = false;
                break;
            }
        }
        if (all_prime) return k;
        ++k;
    }
}
