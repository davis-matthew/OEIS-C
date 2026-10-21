/*
 * A061217  Number of zeros in the concatenation n(n-1)(n-2)(n-3)...321.
 * https://oeis.org/A061217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = total number of digit 0 occurrences in the decimal *
 *            representations of the numbers 1,2,…,n. The computation is
 *            performed * by iterating over each integer and counting its zero
 *            digits directly
 * Range:     u8 n <= 1049, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of zeros in the concatenation of n, n‑1, … , 1. */
seq_t a(idx_t n)
{
    seq_t total = 0;         /* accumulate the zero count */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;          /* copy the current number */
        while (x > 0) {
            if (x % 10 == 0)  /* zero digit found */
                total += 1;
            x /= 10;          /* move to next digit */
        }
    }
    return total;
}
