/*
 * A183294  Complement of A005449.
 * https://oeis.org/A183294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183294: Complement of A005449.
   Formula: a(n) = n + A180447(n-1).
            Equivalent explicit formula:
             m = floor(sqrt(2n/3));
             a(n) = n + m + 1   if 2n >= m(3m+5)+4
             a(n) = n + m       otherwise
   All arithmetic is performed in types seq_t and idx_t. */
#include <stdint.h>

/* Integer square‐root of an unsigned 64-bit number.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_idx(idx_t x)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i)
    {
        idx_t bit = ((idx_t)1) << i;
        idx_t candidate = r | bit;
        /* candidate*candidate may overflow 64 bits, use 128 bits for test. */
        if (( (__int128)candidate * candidate) <= x)
            r = candidate;
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute m = floor(sqrt(2n/3)).  Use 128‑bit to avoid overflow of 2n. */
    __int128 tmp128 = (__int128)n * 2;
    tmp128 /= 3;                     /* now 2n/3 */
    idx_t tmp   = (idx_t)tmp128;      /* fits in 64 bits */
    idx_t m     = isqrt_idx(tmp);     /* m fits in idx_t */

    /* Compute comparison value:  m(3m+5)+4  */
    seq_t m_seq   = (seq_t)m;
    seq_t left    = m_seq * ( (seq_t)3 * m_seq + 5 ) + 4;

    /* Compare 2n with left */
    seq_t two_n   = (seq_t)n * 2;
    seq_t result;
    if (two_n >= left)
        result = (seq_t)n + m_seq + 1;
    else
        result = (seq_t)n + m_seq;

    return result;
}
