/*
 * A078822  Number of distinct binary numbers contained as substrings in the
 *          binary representation of n.
 * https://oeis.org/A078822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = number of distinct binary numbers contained as substrings
 *            in the binary representation of n. Computation is done by
 *            enumerating all substrings and collecting distinct values
 * Range:     u8 n <= 604, u16 n <= 9604, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline unsigned int bitlen_uint64(uint64_t x) {
    /* Return the number of bits needed to represent x.
       For x = 0 we return 1 to match the definition a(0)=1. */
    if (x == 0) return 1;
#if defined(__GNUC__) || defined(__clang__)
    return 64 - __builtin_clzll(x);
#else
    unsigned int len = 0;
    while (x) { len++; x >>= 1; }
    return len;
#endif
}

seq_t a(idx_t n) {
    /* Binary length of n.  For n==0 the length is defined as 1. */
    unsigned int bl = bitlen_uint64((uint64_t)n);

    /* The maximum number of distinct substrings is bl*(bl+1)/2, which
       for bl <= 64 is at most 2080.  We keep a small stack buffer
       to store the distinct values. */
    seq_t seen[2100];
    unsigned int cnt = 0;

    /* Iterate over all starting positions of substrings. */
    for (unsigned int i = 0; i < bl; ++i) {
        seq_t val = 0;
        /* Build substrings starting at i by extending to the right. */
        for (unsigned int j = i; j < bl; ++j) {
            /* Extract bit at position j (most significant bit has
               index 0). */
            unsigned int bit = (n >> (bl - 1 - j)) & 1ULL;
            val = (val << 1) | bit;

            /* Check whether val is already in the list. */
            unsigned int found = 0;
            for (unsigned int k = 0; k < cnt; ++k) {
                if (seen[k] == val) { found = 1; break; }
            }
            if (!found) {
                seen[cnt++] = val;
            }
        }
    }

    return (seq_t)cnt;
}
