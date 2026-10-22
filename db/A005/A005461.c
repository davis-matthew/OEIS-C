/*
 * A005461  Number of simplices in barycentric subdivision of n-simplex.
 * https://oeis.org/A005461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(n+3)!/48 = (n*(n+1)/2) * Product_{i=5..n+3} i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t;
    idx_t i;
    if (n % 2 == 0)
        t = (seq_t)(n / 2) * ((seq_t)n + 1);
    else
        t = (seq_t)n * ((seq_t)((n + 1) / 2));
    for (i = 5; i <= n + 3; i++)
        t *= (seq_t)i;
    return t;
}
