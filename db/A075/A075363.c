/*
 * A075363  Triangle read by rows, in which n-th row gives n smallest powers
 *          of n.
 * https://oeis.org/A075363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r^k where r is the smallest integer such that r(r+1)/2 ≥
 *            n and k = n – r(r–1)/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    /* exponentiation by squaring */
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; keep a defensive guard */

    /* find r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* position within the row (exponent k) */
    unsigned __int128 prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;

    return pow_seq((seq_t)r, k);
}
