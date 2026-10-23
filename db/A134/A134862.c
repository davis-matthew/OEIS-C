/*
 * A134862  Wythoff ABB numbers.
 * https://oeis.org/A134862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A(B(B(n))) with A = A000201, B = A001950. A(n) = ⌊n·φ⌋,
 *            B(n) = A(n) + n, φ = (1+√5)/2. Computes A(n) via integer
 *            arithmetic: A(n) = ⌊(n + ⌊n·√5⌋)/2⌋, and ⌊n·√5⌋ = ⌊√(5·n²)⌋
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer (floor). */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = 1;
    while (high * high <= x) {
        low = high;
        high <<= 1;
    }
    while (high - low > 1) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Lower Wythoff sequence A(n) = ⌊n·φ⌋. */
static inline seq_t wythoff_A(idx_t n)
{
    /* Compute ⌊n·√5⌋ as ⌊√(5·n²)⌋ using 128‑bit arithmetic. */
    unsigned __int128 nn = n;
    unsigned __int128 sq = (unsigned __int128)5 * nn * nn;
    unsigned __int128 sqrt5n = isqrt128(sq);
    /* A(n) = ⌊(n + √(5·n²))/2⌋. */
    unsigned __int128 sum = nn + sqrt5n;
    return (seq_t)(sum / 2);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Step 1: compute B(n) = A(n) + n. */
    seq_t A_n = wythoff_A(n);
    idx_t B_n = (idx_t)(A_n + n);

    /* Step 2: compute B(B(n)) = A(B(n)) + B(n). */
    seq_t A_Bn = wythoff_A(B_n);
    idx_t B_Bn = (idx_t)(A_Bn + B_n);

    /* Step 3: return A(B(B(n))). */
    return wythoff_A(B_Bn);
}
