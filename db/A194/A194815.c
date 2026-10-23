/*
 * A194815  Number of integers k in [1,n] such that {n*r+k*r} < {n*r-k*r},
 *          where { } = fractional part and r=sqrt(2).
 * https://oeis.org/A194815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194815
 *
 *  a(n) = #{ 1 ≤ k ≤ n : { (n+k)√2 } < { (n-k)√2 } }
 *
 *  A direct test of the inequality
 *      { (n+k)√2 } < { (n-k)√2 }
 *  is equivalent to
 *      (⌊(n+k)√2⌋ − ⌊(n-k)√2⌋)^2 > 8k^2
 *  because 2k√2 < Δ   ⇔   8k² < Δ²,  where Δ = difference of the floors.
 *
 *  The algorithm below computes ⌊m√2⌋ exactly using integer arithmetic:
 *      ⌊m√2⌋ = ⌊√(2m²)⌋
 *
 *  No floating‑point operations are used.
 */

#include <stddef.h>
#include <stdint.h>

/* integer square root: floor( sqrt(x) ) for 0 ≤ x < 2^64 */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of four ≤ x */

    while (bit > x)                     /* shift down until bit ≤ x */
        bit >>= 2;

    while (bit) {                       /* main loop */
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/*  Public function required by the build system.                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n=0, safe return */

    seq_t count = 0;                     /* the answer */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute ⌊(n+k)√2⌋ and ⌊(n-k)√2⌋ using ⌊√(2m²)⌋ */
        idx_t m1 = n + k;
        idx_t m2 = n - k;

        uint64_t val1 = 2ULL * m1 * m1;   /* 2*m1*m1 fits in 64 bits for n ≤ 10⁴ */
        uint64_t val2 = 2ULL * m2 * m2;

        seq_t f1 = (seq_t)isqrt64(val1);
        seq_t f2 = (seq_t)isqrt64(val2);

        seq_t delta = f1 - f2;            /* Δ = f1 − f2 */

        /* test Δ² > 8k² using 64–bit arithmetic */
        uint64_t left  = (uint64_t)delta * (uint64_t)delta;
        uint64_t right = 8ULL * (uint64_t)k * (uint64_t)k;

        if (left > right)
            ++count;
    }

    return count;
}
