/*
 * A154618  Triangle read by rows: integer values of T(n,m) =
 *          (4*m*n+2*m+2*n-3)/3.
 * https://oeis.org/A154618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154618:
   a(n) = nth integer entry of the triangle
          T(n,m) = (4*m*n + 2*m + 2*n - 3) / 3  (1 ≤ m ≤ n, n ≥ 1),
   read by rows.  Rows with n ≡ 1 (mod 3) contain no integers.
   In a row with n ≡ 0 (mod 3) the integer entries occur at m =
   3, 6, 9, …; in a row with n ≡ 2 (mod 3) at m = 2, 5, 8, ….
   The returned value is computed using only integer arithmetic. */

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n < 1; return 0 */
        return 0;

    idx_t row = 1;             /* current row of the triangle   */
    idx_t cumulative = 0;      /* number of integer entries seen so far */

    /* Find the row that contains the nth integer entry. */
    while (1)
    {
        idx_t cnt = 0;                 /* integers in this row */
        switch (row % 3)
        {
            case 0: cnt = row / 3;             break;   /* m ≡ 0 (mod 3) */
            case 2: cnt = (row + 1) / 3;       break;   /* m ≡ 2 (mod 3) */
            default: /* row % 3 == 1 */                  break;   /* none */
        }
        if (cumulative + cnt >= n)          /* n lies in this row   */
            break;
        cumulative += cnt;                  /* skip this row        */
        ++row;
    }

    /* position of the desired integer within the found row */
    idx_t pos_in_row = n - cumulative;          /* 1‑based index        */
    idx_t start_m    = (row % 3 == 0) ? 3 : 2;   /* first m producing an integer */
    idx_t m          = start_m + 3 * (pos_in_row - 1);

    /* compute T(row,m) = (4*m*n + 2*m + 2*n - 3) / 3 */
    seq_t val =
        (((seq_t)4 * m * row) + ((seq_t)2 * m) + ((seq_t)2 * row) - (seq_t)3) / (seq_t)3;

    return val;
}
