/*
 * A132748  a(n) = the sum of the positive non-isolated divisors of n.
 * https://oeis.org/A132748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the positive non‑isolated divisors of n. A divisor
 *            d of n is non‑isolated iff (d-1) | n or (d+1) | n
 * Range:     u8 n <= 5939, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Odd numbers have no non‑isolated divisors */
    if ((n & 1U) == 1U)
        return 0;

    seq_t sum = 0;
    /* Enumerate all divisors d of n in pairs (i, n/i) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        /* Process divisor i */
        if ((i > 1 && n % (i - 1) == 0) || (n % (i + 1) == 0))
            sum += (seq_t)i;

        idx_t j = n / i;
        if (j == i)                 /* handled already */
            continue;

        /* Process the paired divisor j = n/i */
        if ((j > 1 && n % (j - 1) == 0) || (n % (j + 1) == 0))
            sum += (seq_t)j;
    }
    return sum;
}
