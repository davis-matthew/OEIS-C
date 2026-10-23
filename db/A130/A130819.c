/*
 * A130819  2*k appears 2*k-1 times.
 * https://oeis.org/A130819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil(sqrt(n)) for n >= 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for offset 1, safe guard */

    idx_t t = n - 1;           /* compute floor_sqrt(t) where t = n-1 */

    /* Find an upper bound on sqrt(t) */
    idx_t low = 0, high = 1;
    while (high <= t / high)
        high <<= 1;

    /* Binary search for floor(sqrt(t)) */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= t / mid)
            low = mid;
        else
            high = mid;
    }

    idx_t k = low + 1;         /* ceil(sqrt(n)) */
    return (seq_t)2 * (seq_t)k;
}
