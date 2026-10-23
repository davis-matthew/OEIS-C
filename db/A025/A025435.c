/*
 * A025435  Number of partitions of n into 2 distinct squares.
 * https://oeis.org/A025435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (x,y) with x<y, x^2 + y^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of a 64‑bit unsigned integer
   (returns floor(sqrt(n)) ).  It uses only integer arithmetic. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0)
                return 0;
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)        /* 0 cannot be represented as two distinct squares */
        return 0;

    seq_t count = 0;
    idx_t limit = _isqrt_u64(n);   /* largest a with a^2 <= n */
    for (idx_t a = 0; a < limit; ++a) {
        unsigned __int128 a2 = (unsigned __int128)a * (unsigned __int128)a;
        if (a2 >= (unsigned __int128)n)   /* a^2 can't exceed n */
            break;
        idx_t b2 = n - (idx_t)a2;
        idx_t b = _isqrt_u64(b2);
        if ((unsigned __int128)b * (unsigned __int128)b == (unsigned __int128)b2 &&
            b > a)            /* distinct and unordered */
            ++count;
    }
    return count;
}
