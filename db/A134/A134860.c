/*
 * A134860  Wythoff AAB numbers; also, Fib101 numbers: those n for which the
 *          Zeckendorf expansion A014417(n) ends with 1,0,1.
 * https://oeis.org/A134860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A(A(B(n))) = 3*A(n) + 2*n - 1 * where A(n) = ⌊ n*φ ⌋ with
 *            φ = (1+√5)/2. * A(n) can be computed by A(n) = ⌊ ( n + ⌊√(5*n²)⌋
 *            ) / 2 ⌋, * so we only need an integer square‑root of 5*n²
 * Range:     u8 n <= 37, u16 n <= 9561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binary search integer square root for 128‑bit values.
   Returns ⌊√x⌋ for x ≥ 0. */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)-1;          /* 2⁶⁴−1 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

extern seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* index 0 is unused in OEIS */
    /* Compute A(n) = ⌊ (n + ⌊√(5*n²)⌋ ) / 2 ⌋ */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * nn * (unsigned __int128)5;      /* 5*n² */
    uint64_t sqrt5n2 = isqrt_u128(prod);                          /* ⌊√(5*n²)⌋ */
    unsigned __int128 A = (nn + (unsigned __int128)sqrt5n2) / 2;   /* A(n) */
    /* Compute a(n) = 3*A + 2*n - 1 */
    unsigned __int128 res = (unsigned __int128)3 * A + (unsigned __int128)2 * nn - (unsigned __int128)1;
    return (seq_t)res;
}
