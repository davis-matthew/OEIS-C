/*
 * A104894  Binary array, related to the Thue-Morse sequence A010060, read by
 *          downward antidiagonals.
 * https://oeis.org/A104894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A010060( (d - (n - d(d+1)/2)) mod 2^((n - d(d+1)/2)+1) ),
 *            where d = ⌊(√(8·n + 1) – 1)/2⌋. This is equivalent to a(n) =
 *            A010060(c mod 2^(r+1)) with r = n – d(d+1)/2 , c = d – r
 * Range:     u8 n <= 9488, u16 n <= 9488, u32 n <= 9488, u64 n <= 9488,
 *            u128 n <= 9488, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit value, returning the floor. */
static inline uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 63, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Compute a(n) for OEIS A104894. */
seq_t a(idx_t n)
{
    /* d = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 eight_n_plus1 = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt128(eight_n_plus1);
    uint64_t d = (s - 1) >> 1;

    /* start index of diagonal d */
    unsigned __int128 start128 = (unsigned __int128)d * (d + 1) / 2;
    uint64_t start = (uint64_t)start128;

    /* row = offset within diagonal; col = d - row */
    uint64_t row = n - start;
    uint64_t col = d - row;

    /* Thue–Morse argument: col mod 2^(row+1) */
    uint64_t k;
    if (row + 1 >= 64) {
        k = col;                     /* modulus larger than col */
    } else {
        uint64_t mask = ((uint64_t)1 << (row + 1)) - 1;
        k = col & mask;
    }

    /* parity of popcount of k gives 0 for even, 1 for odd */
    uint64_t parity = __builtin_parityll(k);

    return (seq_t)parity;
}
