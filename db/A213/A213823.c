/*
 * A213823  Principal diagonal of the convolution array A213822.
 * https://oeis.org/A213823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-n - 3*n^2 + 12*n^3)/2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to a 64‑bit unsigned integer for intermediate
       calculation; this is safe for all n used in the tests (n ≤ ~10^4). */
    unsigned long long u = n;
    unsigned long long num = 12ULL * u * u * u
                            - 3ULL * u * u
                            - u;
    return (seq_t)(num / 2);
}
