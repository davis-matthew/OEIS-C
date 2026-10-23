/*
 * A130478  Triangle T(n,k) = n! / A130477(n,k).
 * https://oeis.org/A130478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑major enumeration of the triangle whose n‑th row
 *            consists of the n terms [ n! ; (n-1)!+(n-2)! ; (n-2)!+(n-3)! ;
 *            ... ; 1!+0! ]
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n > (r-1)*r/2. */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* Position i within this row (1-based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t i = n - prev;          /* 1 <= i <= r */

    /* If first entry of the row, return r! directly. */
    if (i == 1)
    {
        seq_t fact = 1;
        for (idx_t k = 2; k <= r; ++k)
            fact *= (seq_t)k;
        return fact;
    }

    /* For the remaining entries compute (j)! + (j-1)!,
       where j = r - i + 1 is the first term of the sum. */
    idx_t j = r - i + 1;          /* 1 <= j <= r-1 */

    seq_t fact_j = 1;
    for (idx_t k = 2; k <= j; ++k)
        fact_j *= (seq_t)k;       /* j! */

    seq_t fact_j_minus1 = fact_j / (seq_t)j;  /* (j-1)! because j! / j */
    return fact_j + fact_j_minus1;
}
