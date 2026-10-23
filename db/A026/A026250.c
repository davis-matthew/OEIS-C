/*
 * A026250  Beginning with the natural numbers, swap [ k*sqrt(2) ] and [ k*(2
 *          + sqrt(2)) ], for all k >= 1.
 * https://oeis.org/A026250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) swaps floor(k*sqrt(2)) and floor(k*(2+sqrt(2))) for all k
 *            >= 1. This implementation determines the Beatty sequence index k
 *            for a given n by binary search and then returns the
 *            corresponding swapped value
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit values. */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low = 0, high = (uint64_t)1 << 63, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 msq = (unsigned __int128)mid * mid;
        if (msq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* floor(k*sqrt(2)) using integer arithmetic: floor(sqrt(2*k*k)). */
static inline uint64_t beatty_s(idx_t k)
{
    unsigned __int128 val = (unsigned __int128)k * k;
    val <<= 1;                       /* val = 2*k*k */
    return isqrt_u128(val);
}

/* Binary search for k such that beatty_s(k) == n.  Returns 0 if none. */
static inline idx_t find_k_in_s(idx_t n)
{
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        uint64_t s = beatty_s(mid);
        if (s == n) return mid;
        if (s < n) low = mid + 1;
        else high = mid - 1;
    }
    return 0;
}

/* Binary search for k such that 2*k + beatty_s(k) == n.  Returns 0 if none. */
static inline idx_t find_k_in_t(idx_t n)
{
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        uint64_t s = beatty_s(mid);
        unsigned __int128 t = (unsigned __int128)2 * mid + s;
        if (t == n) return mid;
        if (t < n) low = mid + 1;
        else high = mid - 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* First try the sqrt(2) Beatty sequence. */
    idx_t k = find_k_in_s(n);
    if (k != 0) {
        /* n == beatty_s(k), answer is 2*k + n. */
        unsigned __int128 val = (unsigned __int128)2 * k + n;
        return (seq_t)val;
    }
    /* Otherwise n is in the (2+sqrt(2)) Beatty sequence. */
    k = find_k_in_t(n);
    /* It must exist because the two sequences are complementary. */
    return (seq_t)beatty_s(k);
}
