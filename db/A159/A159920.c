/*
 * A159920  Sums of the antidiagonals of Sundaram's sieve (A159919).
 * https://oeis.org/A159920
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+4)/3
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2)
        return 0;
    seq_t nn = (seq_t)n;
    return ((nn - 1) * nn * (nn + 4)) / 3;
}
