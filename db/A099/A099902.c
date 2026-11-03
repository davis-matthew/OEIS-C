/*
 * A099902  Multiplies by 2 and shifts right under the XOR BINOMIAL transform
 *          (A099901).
 * https://oeis.org/A099902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(n) = Σ_{k=0..n} (binomial(n-k+⌊k/2⌋, ⌊k/2⌋) mod 2) * 2^k *
 *            Using Lucas’ theorem parity condition: binomial(i,j) is odd ⇔ j
 *            & (i-j) == 0 * Here i = n-k+⌊k/2⌋, i-j = n-k, so parity
 *            condition becomes * ⌊k/2⌋ & (n-k) == 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 3242 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulated sum */
    seq_t pow2   = 1;          /* 2^k for current k */
    for (idx_t k = 0; k <= n; ++k) {
        idx_t j   = k >> 1;    /* ⌊k/2⌋ */
        idx_t diff = n - k;    /* i - j = n - k */
        if ((j & diff) == 0)   /* parity of binomial is 1 */
            result += pow2;    /* add 2^k to the sum */
        /* prepare 2^(k+1) for next iteration */
        pow2 = mul_small(pow2, 2);
    }
    return result;
}
