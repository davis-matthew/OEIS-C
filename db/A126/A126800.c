/*
 * A126800  Smallest divisor of n which is greater than largest divisor d of n
 *          such that each integer from 1 to d is also a divisor of n.
 * https://oeis.org/A126800
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n greater than the largest d such
 *            that every integer from 1 to d divides n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3.
       For convenience return 0 for other n. */
    if (n < 3u) return 0;

    /* Find d: the largest integer such that 1..d all divide n.
       This is the first integer k > 1 that does NOT divide n,
       then d = k-1. */
    idx_t k;
    for (k = 2; ; ++k) {
        if (n % k != 0) break;
        /* In practice k will never reach n+1 for the values of n
           we handle, but the loop stops safely when the first missing
           divisor is encountered. */
        if (k > n) break;
    }
    seq_t d = (seq_t)(k - 1);

    /* Find the smallest divisor of n that exceeds d.
       Enumerate divisors up to sqrt(n). */
    seq_t best = (seq_t)n;   /* n itself is always a divisor > d. */

    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t div1 = (seq_t)i;
            seq_t div2 = (seq_t)(n / i);
            if (div1 > d && div1 < best)
                best = div1;
            if (div2 > d && div2 < best)
                best = div2;
        }
    }
    return best;
}
