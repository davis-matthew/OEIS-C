/*
 * A205136  Ordered differences of distinct pentagonal numbers.
 * https://oeis.org/A205136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(j)-s(i), where s(k)=k*(3k-1)/2, j =
 *            ceil((1+sqrt(1+8n))/2) and i = n - (j-1)*(j-2)/2, so 1 <= i < j
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the smallest j such that j*(j-1)/2 >= n. */
    idx_t j = 1;
    while ( (j * (j - 1)) / 2 < n )
        ++j;

    /* Compute i, the position inside the j‑th block. */
    idx_t prev = ( (j - 1) * (j - 2) ) / 2;
    idx_t i = n - prev;          /* 1 <= i < j */

    /* Compute the pentagonal numbers as seq_t. */
    seq_t sj = ((seq_t)j * ((((seq_t)j) * 3) - 1)) / 2;
    seq_t si = ((seq_t)i * ((((seq_t)i) * 3) - 1)) / 2;

    return sj - si;
}
