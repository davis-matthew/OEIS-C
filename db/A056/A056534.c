/*
 * A056534  Mapping from the ordering by product (A027750, A056538) to the
 *          ordering by sum (A002260, A004736) of ordered pairs (a,b), a>=1,
 *          b>=1.
 * https://oeis.org/A056534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 2192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056534: mapping from the ordering by product to the ordering by sum.
    For the n-th pair (a,b) in product order, its position in sum order is
    a(n) = ((a+b-1)*(a+b-2))/2 + a.
    The code enumerates pairs in product order by increasing product,
    with ties broken by increasing first coordinate.  For each pair it
    computes the above formula until the n-th pair is reached. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t prod = 1; ; ++prod)            /* iterate product from 1 upwards */
    {
        for (idx_t a1 = 1; a1 <= prod; ++a1)  /* enumerate divisors a1 of prod */
        {
            if (prod % a1 != 0)
                continue;
            idx_t b1 = prod / a1;             /* corresponding second coordinate */
            idx_t s = a1 + b1;                /* sum of the pair */
            seq_t index = ((seq_t)(s - 1) * (s - 2)) / 2 + (seq_t)a1;
            ++count;
            if (count == n)
                return index;                 /* n-th pair found */
        }
    }
}
