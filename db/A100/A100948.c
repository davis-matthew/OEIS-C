/*
 * A100948  Irregular triangle with T(n,1) = floor(n!/3), T(n,2) = n!/2,
 *          T(n,3) = n!, read by rows.
 * https://oeis.org/A100948
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(r,c) where r = floor((n-2)/3)+2, c = ((n-2) mod 3)+1;
 *            T(r,1) = ceil(r! / 3), T(r,2) = r! / 2, T(r,3) = r!
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n - 2;
    idx_t r = k / 3 + 2;
    idx_t c = k % 3 + 1;

    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i) {
        fact *= (seq_t)i;
    }

    switch (c) {
        case 1: return (fact + 2) / 3;
        case 2: return fact / 2;
        case 3: return fact;
    }
    return 0; /* unreachable */
}
