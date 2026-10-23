/*
 * A024163  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that c - b < b - a.
 * https://oeis.org/A024163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of nonnegative integer solutions (x,y,z) of 2·x +
 *            3·y + 6·z = n – 11, i.e. the number of partitions of n‑11 into
 *            parts 2, 3 and 6
 * Range:     u8 n <= 142, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 11 there is no solution (negative right‑hand side). */
    if (n < 11)
        return 0;

    /* N = n – 11 (right‑hand side of the equation). */
    seq_t N = (seq_t)n - 11;

    seq_t count = 0;

    /* Iterate over z such that 6·z ≤ N.  The loop variable z is idx_t
       because its range is at most N/6; we cast to seq_t when multiplying
       by 6. */
    for (idx_t z = 0; (seq_t)z * 6 <= N; ++z)
    {
        /* Remaining value after fixing z.  rem1 = N – 6·z. */
        seq_t rem1 = N - (seq_t)z * 6;

        /* y must satisfy: 0 ≤ y ≤ rem1/3 and y has the same parity
           as rem1 (because 3·y must have the same parity as rem1
           for rem1 – 3·y to be even and thus divisible by 2). */
        seq_t ymax = rem1 / 3;     /* maximum possible y */
        seq_t ypar = rem1 % 2;     /* required parity of y */

        if (ymax >= ypar)
        {
            /* Count of y values with correct parity:
               y = ypar, ypar+2, … , ymax.  Count = ((ymax‑ypar)/2)+1. */
            seq_t add = ((ymax - ypar) / 2) + 1;
            count += add;
        }
    }

    return count;
}
