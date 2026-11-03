/*
 * A140579  Triangle read by rows, A014963(n) * 0^(n-k); 1<=k<=n.
 * https://oeis.org/A140579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 unless n is a triangular number n = r(r+1)/2. If it is
 *            triangular, a(n) = A014963(r): the unique prime factor of r when
 *            r is a prime power, and 1 otherwise (A014963(1) = 1). This
 *            implements the OEIS A140579 sequence
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 107 terms through n = 4782 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* A014963(n): returns the unique prime factor of n if n is a prime power,
   otherwise returns 1.  For n<=1 the value is 1. */
static seq_t A014963_of(idx_t n) {
    if (n <= 1) return 1;
    idx_t tmp = n;

    /* check factor 2 */
    if ((tmp & 1) == 0) {
        while ((tmp & 1) == 0) tmp >>= 1;
        return (tmp == 1) ? (seq_t)2 : (seq_t)1;
    }

    /* odd trial division */
    for (idx_t i = 3; (unsigned __int128)i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            while (tmp % i == 0) tmp /= i;
            return (tmp == 1) ? (seq_t)i : (seq_t)1;
        }
    }

    /* remaining tmp is prime */
    return (seq_t)tmp;
}

/* integer square root of an unsigned __int128 value */
static uint64_t isqrt128(unsigned __int128 x) {
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return (uint64_t)low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1, ignore n=0 */

    /* check if n is triangular */
    unsigned __int128 val = ((unsigned __int128)n) * 8 + 1;
    uint64_t root = isqrt128(val);
    idx_t r = (root - 1) / 2;  /* triangle index */

    unsigned __int128 tri = ((unsigned __int128)r * (r + 1)) / 2;
    if (tri != n) return 0;   /* non‑triangular position */

    return A014963_of(r);
}
