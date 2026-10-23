/*
 * A147645  Number of distinct Mersenne primes dividing n.
 * https://oeis.org/A147645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct Mersenne primes dividing n (i.e.,
 *            number of primes of the form 2^p–1, with prime p, that divide n)
 * Range:     u8 n <= 1202, u16 n <= 8756, u32 n <= 8756, u64 n <= 8756,
 *            u128 n <= 8756, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Helper: bit length of a seq_t value. */
static inline idx_t seq_bitlen(seq_t x) {
    idx_t len = 0;
    while (x != 0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Return the number of distinct Mersenne primes dividing n. */
seq_t a(idx_t n) {
    /* Known exponents of Mersenne primes up to 127 (the largest that fit
       into 128‑bit integers). Other, larger exponents would exceed the
       bit width of seq_t in a 64‑bit build and are never needed for
       n ≤ 2^64‑1. */
    static const uint16_t exponents[] = {
        2,3,5,7,13,17,19,31,61,89,107,127
    };
    const size_t ex_cnt = sizeof(exponents)/sizeof(exponents[0]);

    /* Compute bit length of n.  For seq_t 128‑bit, this is safe; for
       arbitrary‑precision, the loop works as long as shift is defined. */
    idx_t bits = seq_bitlen((seq_t)n);

    seq_t count = 0;                      /* result */
    for (size_t i = 0; i < ex_cnt; ++i) {
        uint16_t p = exponents[i];
        if (p > bits) break;              /* 2^p > n+1 → 2^p–1 cannot divide n */

        seq_t mers = pow2(p) - (seq_t)1;   /* 2^p – 1 */
        if (mers > (seq_t)n) break;        /* safety guard for very large n */

        if (((seq_t)n) % mers == 0)
            ++count;
    }
    return count;
}
