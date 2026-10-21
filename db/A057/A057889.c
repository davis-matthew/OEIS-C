/*
 * A057889  Bijective bit-reverse of n: keep the trailing zeros in the binary
 *          expansion of n fixed, but reverse all the digits up to that point.
 * https://oeis.org/A057889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = A030101(A000265(n)) * A006519(n) (i.e. reverse the binary
 *            digits of the odd part of n, then multiply by the power of two
 *            given by the number of trailing zeros)
 * Range:     u8 n <= 255, u16 n <= 9433, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count trailing zero bits of an unsigned 64‑bit integer.
   For n == 0 the function returns 64. */
static unsigned count_trailing_zeros_idx(idx_t n)
{
    unsigned c = 0;
    while ((n & 1ULL) == 0ULL && c < 64) {
        n >>= 1;
        ++c;
    }
    return c;   /* for n==0, c==64 */
}

/* Length (floor(log2(x))+1) of the binary representation of x.
   For x == 0 the function returns 0. */
static unsigned bitlen_idx(idx_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Reverse the lowest 'len' bits of 'x'.
   Assumes 0 <= len <= 64. */
static seq_t reverse_bits(seq_t x, unsigned len)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        if (x & ((seq_t)1 << i))
            rev |= ((seq_t)1 << (len - 1 - i));
    }
    return rev;
}

/* OEIS sequence A057889 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) = 0 */

    /* 1. Count trailing zeros of n. */
    unsigned tz = count_trailing_zeros_idx(n);

    /* 2. Remove trailing zeros → odd part m. */
    idx_t m = n >> tz;                /* m is odd (except for n=0, handled above) */

    /* 3. Length of m in bits. */
    unsigned len = bitlen_idx(m);

    /* 4. Reverse the bits of the odd part. */
    seq_t rev = reverse_bits((seq_t)m, len);

    /* 5. Restore the trailing zeros. */
    return rev << tz;
}
