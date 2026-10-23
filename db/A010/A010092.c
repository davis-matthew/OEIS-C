/*
 * A010092  Weight distribution of d=4 Hamming code of length 63.
 * https://oeis.org/A010092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A010087(2n) – weight distribution of the d=4 Hamming code
 *            of length 63. The closed form is a(n) = ( C(63,2n) + 63 *
 *            K_{2n}(32) ) / 64 where K_w(32) = Σ_{i=max(0,w-31)}^{min(w,32)}
 *            (-1)^i * C(32,i) * C(31,w-i)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for 0 <= n <= 63 using 128‑bit
   intermediate to avoid overflow.  The result always fits in uint64_t. */
static uint64_t binom_ui(idx_t n, idx_t k)
{
    if (k > n)            return 0;
    if (k > n - k)        k = n - k;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return (uint64_t)r;
}

seq_t a(idx_t n)
{
    /* weight w = 2n */
    idx_t w = n * 2;

    /* C(63,w) */
    uint64_t c63w = binom_ui(63, w);

    /* Krawtchouk sum for (32, w) */
    signed __int128 ksum = 0;
    idx_t i_min = (w > 31) ? (w - 31) : 0;
    idx_t i_max = (w < 32) ? w : 32;
    for (idx_t i = i_min; i <= i_max; ++i) {
        uint64_t b1 = binom_ui(32, i);
        uint64_t b2 = binom_ui(31, w - i);
        signed __int128 term = (signed __int128)b1 * b2;
        if (i & 1)
            ksum -= term;
        else
            ksum += term;
    }

    /* Numerator = C(63,w) + 63 * K_{w}(32)   (use 128‑bit to be safe) */
    signed __int128 num = (signed __int128)c63w + (signed __int128)63 * ksum;

    /* Divide by 64 (shift right by 6) – the result is non‑negative. */
    uint64_t result = (uint64_t)((unsigned __int128)num >> 6);

    return (seq_t)result;
}
