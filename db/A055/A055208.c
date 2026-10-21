/*
 * A055208  Table read by ascending antidiagonals: T(n,k) (n >= 1, k >= 1) is
 *          the sum of k-th powers of digits of n.
 * https://oeis.org/A055208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A055208: Table read by ascending antidiagonals.
   T(n,k) = sum of the k‑th powers of the decimal digits of n.
   The sequence a(n) lists T(i,j) for all i≥1, j≥1 scanned by
   antidiagonals with i decreasing from the diagonal end. */

seq_t a(idx_t n)
{
    /* find the smallest D such that D*(D+1)/2 ≥ n   (D ≥ 1) */
    idx_t D = 0;
    while (((unsigned __int128)D * (D + 1)) / 2 < n)
        ++D;

    /* number of terms strictly before this diagonal */
    idx_t prev = ((D - 1) * D) / 2;

    /* 0‑based offset within the diagonal */
    idx_t offset = n - prev - 1;

    /* coordinates (i,j) in the table */
    idx_t i = D - offset;          /* n coordinate (row)   */
    idx_t j = offset + 1;          /* k coordinate (column) */

    /* compute the sum of j‑th powers of the decimal digits of i */
    seq_t sum = 0;
    idx_t tmp = i;

    while (tmp > 0) {
        unsigned digit = tmp % 10;
        seq_t power = 1;
        for (idx_t p = 0; p < j; ++p)
            power *= (seq_t)digit;
        sum += power;
        tmp /= 10;
    }

    return sum;
}
