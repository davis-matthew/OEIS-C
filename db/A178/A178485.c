/*
 * A178485  (A178475(n)-6)/9.
 * https://oeis.org/A178485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, easy
 * Formula:   a(n) = (perm(n)-6)/9 where perm(n) is the n-th permutation of
 *            the digits 1,2,3,4,5 in ascending order (index 1)
 * Range:     u8 none, u16 n <= 120, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has 120 terms – repeat it if n is larger. */
    const idx_t TOTAL = 120;
    /* Convert n to the 1..120 range. */
    idx_t idx = (n - 1) % TOTAL;          /* 0‑based index in [0,119]    */
    idx_t idx0 = idx;                     /* mutable copy for factorial form */

    /* Available digits, initially sorted. */
    int digits[5] = { 1, 2, 3, 4, 5 };

    /* Pre‑computed factorials 0! … 5! . */
    static const idx_t factorial[6] = { 1, 1, 2, 6, 24, 120 };

    seq_t perm = 0;   /* will accumulate the permutation number */

    for (int pos = 0; pos < 5; pos++)
    {
        /* Factorial of the remaining positions. */
        idx_t fact = factorial[5 - pos - 1];        /* 24, 6, 2, 1, 1 */

        /* Which digit to pick next. */
        idx_t fidx = idx0 / fact;
        idx0   %= fact;

        int d = digits[(int)fidx];

        /* Append the digit to the number. */
        perm = perm * 10 + (seq_t)d;

        /* Remove the used digit from the list. */
        for (int j = (int)fidx; j < 4 - pos; j++)
            digits[j] = digits[j + 1];
    }

    /* All such numbers are 9k+6; thus (perm-6)/9 is an integer. */
    return (perm - 6) / 9;
}
