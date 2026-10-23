/*
 * A154724  Triangle read by rows in which row n lists 2n-1 terms: n, in the
 *          center of the row and the pairs of prime numbers that are
 *          equidistant to n, with 0's inserted, as shown below in the
 *          example.
 * https://oeis.org/A154724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the entry at linear index n in the triangle read by rows
 *            described by A154724. For row r (with length 2r‑1) the elements
 *            are arranged in positions 0 … 2r‑2, where position r‑1 holds r.
 *            For any other position i: let d = |i – (r‑1)|. Let L = r – d and
 *            R = r + d. If L and R are both prime, t...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root (floor).  Works for all 64‑bit values. */
static idx_t isqrt_uint64(idx_t n)
{
    idx_t low = 0;
    /* 2^32 fits in a 64‑bit integer and is the maximum possible square root
       of a 64‑bit unsigned integer. */
    idx_t high = (idx_t)1ULL << 32;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {          /* avoid division by zero */
            low = 1;
            continue;
        }
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return low - 1;
}

/* Simple deterministic primality test for 64‑bit integers. */
static bool is_prime_idx(idx_t n)
{
    if (n < 2)             return false;
    if ((n & 1) == 0)      return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0)    return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Determine the row r containing index n. */
    idx_t r = isqrt_uint64(n - 1) + 1;          /* row number (1‑based) */
    idx_t start = (r - 1) * (r - 1) + 1;        /* first index of row r */
    idx_t pos   = n - start;                    /* 0‑based position in row */
    idx_t center = r - 1;                       /* center position offset */

    if (pos == center)                         /* the central element */
        return (seq_t)r;

    idx_t d = (pos > center) ? (pos - center) : (center - pos);
    idx_t left  = r - d;
    idx_t right = r + d;

    if (is_prime_idx(left) && is_prime_idx(right)) {
        if (pos < center)
            return (seq_t)left;   /* left side */
        else
            return (seq_t)right;  /* right side */
    } else {
        return (seq_t)0;           /* not a prime pair */
    }
}
