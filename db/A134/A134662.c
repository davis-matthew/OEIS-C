/*
 * A134662  Number of odd coefficients in (1 + x + x^4)^n.
 * https://oeis.org/A134662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd coefficients in (1 + x + x^4)^n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/*  Return the number of odd coefficients in (1 + x + x^4)^n.  */
seq_t a(idx_t n)
{
    /* maximum number of bits needed: 4*n + 1 (degree up to 4n).  */
    idx_t max_bits = 4 * n + 1;
    size_t words = (max_bits + 63) / 64;      /* number of 64‑bit words */

    /* Allocate bit vectors for current and next polynomial.  */
    uint64_t *dp  = (uint64_t *)malloc(words * sizeof(uint64_t));
    uint64_t *newdp = (uint64_t *)malloc(words * sizeof(uint64_t));
    if (!dp || !newdp) return 0;               /* allocation failure */

    /* initialise: (1 + x + x^4)^0 = 1  */
    memset(dp, 0, words * sizeof(uint64_t));
    dp[0] = 1;

    for (idx_t step = 0; step < n; ++step)
    {
        /*  newdp = dp  (copy)  */
        memcpy(newdp, dp, words * sizeof(uint64_t));

        /*  XOR with dp << 1  */
        uint64_t carry = 0;
        for (size_t w = 0; w < words; ++w)
        {
            uint64_t val = dp[w];
            uint64_t shifted = (val << 1) | carry;
            carry = val >> 63;
            newdp[w] ^= shifted;
        }

        /*  XOR with dp << 4  */
        carry = 0;
        for (size_t w = 0; w < words; ++w)
        {
            uint64_t val = dp[w];
            uint64_t shifted = (val << 4) | carry;
            carry = val >> 60;        /* bits discarded from high 4 positions */
            newdp[w] ^= shifted;
        }

        /*  swap dp and newdp  */
        uint64_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /*  Count the number of 1 bits in dp up to degree 4*n  */
    idx_t bits_used = 4 * n + 1;
    size_t last_word = (bits_used + 63) / 64;
    seq_t count = 0;
    for (size_t w = 0; w < last_word; ++w)
        count += __builtin_popcountll(dp[w]);

    free(dp);
    free(newdp);
    return count;
}
