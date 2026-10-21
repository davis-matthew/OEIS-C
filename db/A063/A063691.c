/*
 * A063691  Number of solutions to x^2 + y^2 + z^2 = n in positive integers.
 * https://oeis.org/A063691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of ordered positive triples (x,y,z) with x^2 + y^2
 *            + z^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find floor(sqrt(v)) for an unsigned integer v   */
static inline idx_t isqrt(idx_t v)
{
    idx_t low = 0;
    idx_t high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == v)
            return mid;
        if (sq < v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;          /* high is floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* x ranges from 1 to floor(sqrt(n)) */
    idx_t xmax = isqrt(n);
    for (idx_t x = 1; x <= xmax; ++x) {
        idx_t x2 = x * x;
        if (x2 > n) break;                /* safety, but should not happen */

        /* remaining after subtracting x^2 */
        idx_t rest_x = n - x2;
        idx_t ymax = isqrt(rest_x);

        for (idx_t y = 1; y <= ymax; ++y) {
            idx_t y2 = y * y;
            if (x2 + y2 > n) break;        /* safety */

            idx_t rem = rest_x - y2;
            if (rem == 0) continue;        /* z must be positive */

            idx_t z = isqrt(rem);
            if (z * z == rem)
                ++count;
        }
    }

    return count;
}
