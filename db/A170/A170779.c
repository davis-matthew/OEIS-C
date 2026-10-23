/*
 * A170779  a(n) = n^8*(n^7 + 1)/2.
 * https://oeis.org/A170779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 20, u128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_safe(seq_t a, seq_t b)
{
    /* When seq_t is a 64‑bit unsigned type in C we use a 128‑bit
       intermediate to avoid overflow.  In C++ or for other sizes
       the ordinary multiplication is fine (big‑int classes overload it). */
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    if (sizeof(seq_t) == 8)
        return (seq_t)((__uint128_t)a * b);
#endif
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;

    seq_t n2 = mul_safe(N, N);
    seq_t n3 = mul_safe(n2, N);
    seq_t n4 = mul_safe(n3, N);
    seq_t n5 = mul_safe(n4, N);
    seq_t n6 = mul_safe(n5, N);
    seq_t n7 = mul_safe(n6, N);
    seq_t n8 = mul_safe(n7, N);

    seq_t result;
    if (N & 1) {                /* n is odd → n^7 + 1 is even */
        seq_t half = (n7 + (seq_t)1) >> 1;
        result = mul_safe(n8, half);
    } else {                     /* n is even → n^8 is even */
        seq_t half = n8 >> 1;
        result = mul_safe(half, n7 + (seq_t)1);
    }
    return result;
}
