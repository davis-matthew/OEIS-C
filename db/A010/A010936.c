/*
 * A010936  Binomial coefficient C(20,n).
 * https://oeis.org/A010936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(20,n) (the nth binomial coefficient with upper index
 *            20)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values are defined only for 0 ≤ n ≤ 20; outside that range the binomial
       coefficient is zero. */
    if (n > 20u) {
        return 0;
    }

    seq_t result = 1;
    /* Compute C(20,n) multiplicatively without intermediate overflow. */
    for (idx_t i = 1; i <= n; ++i) {
        /* (20 - n + i) never underflows here because n ≤ 20. */
        result = result * (seq_t)(20u - n + i) / i;
    }
    return result;
}
