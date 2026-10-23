/*
 * A188550  Maximal number of divisors d>1 of n-k such that n-d is a multiple
 *          of k, when k runs through values 2, 3, ..., floor(sqrt(n)).
 * https://oeis.org/A188550
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{2≤k≤⌊√n⌋} #{ d>1 | d | (n–k) and (n–d) mod k = 0 }
 * Range:     u8 n <= 9105, u16 n <= 9105, u32 n <= 9105, u64 n <= 9105,
 *            u128 n <= 9105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    /* floor(sqrt(n)) using only integer arithmetic */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;          /* undefined for n < 4, but safe */

    idx_t ceil_sqrt = isqrt_u64(n);
    seq_t maxcnt = 0;

    for (idx_t k = 2; k <= ceil_sqrt; ++k) {
        idx_t m = n - k;
        if (m <= 1)
            continue;       /* no divisors >1 */

        seq_t cnt = 0;
        idx_t sqrt_m = isqrt_u64(m);

        for (idx_t d = 2; d <= sqrt_m; ++d) {
            if (m % d == 0) {
                if (((n - d) % k) == 0)
                    ++cnt;

                idx_t com = m / d;
                if (com != d && ((n - com) % k) == 0)
                    ++cnt;
            }
        }

        /* count divisor m itself (needed for primes and composites) */
        if (((n - m) % k) == 0)
            ++cnt;

        if (cnt > maxcnt)
            maxcnt = cnt;
    }

    return maxcnt;
}
