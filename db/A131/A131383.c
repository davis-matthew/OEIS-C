/*
 * A131383  Total digital sum of n: sum of the digital sums of n for all the
 *          bases 1 to n (a 'digital sumorial').
 * https://oeis.org/A131383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=1}^{n} ds_b(n) (digital sum of n in base b)
 * Range:     u8 n <= 32, u16 n <= 596, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              // not used, offset starts at 1

    seq_t total = 0;
    for (idx_t base = 1; base <= n; ++base) {
        if (base == 1) {              // unary representation
            total += (seq_t)n;
            continue;
        }
        idx_t x = n;
        seq_t ds = 0;
        while (x > 0) {
            ds += (seq_t)(x % base);
            x /= base;
        }
        total += ds;
    }
    return total;
}
