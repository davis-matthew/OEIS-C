/*
 * A157457  Read n-th row of triangle in A157458 and regard it as the
 *          expansion of a number in base n+1.
 * https://oeis.org/A157457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A157457: number obtained by reading row n of triangle A157458 in base n+1.
   The n‑th row consists of n+1 digits:
     - For n even: digits are 1,3,5,…,n‑1, n, n‑1,…,5,3,1
     - For n odd :  digits are 1,3,5,…,n‑2, n, n, n‑2,…,5,3,1
   The function constructs the base‑(n+1) number formed by these digits. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) = 0 */
    seq_t base = (seq_t)n + 1;           /* base = n+1 */
    seq_t result = 0;

    /* pre‑compute values used in digit calculation */
    idx_t mid1 = (n % 2 == 0) ? (n / 2) : ((n - 1) / 2);

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t d;                        /* digit at position k */

        if (n % 2 == 0)                /* n even */
        {
            if (k == mid1)              /* central digit */
                d = (seq_t)n;
            else
            {
                idx_t left = (k < mid1) ? k : (n - k);
                d = (seq_t)(2 * left + 1);
            }
        }
        else                            /* n odd */
        {
            if (k == mid1 || k == mid1 + 1) /* two central digits */
                d = (seq_t)n;
            else
            {
                idx_t left = (k < mid1) ? k : (n - k);
                d = (seq_t)(2 * left + 1);
            }
        }

        result = result * base + d;     /* append the digit */
    }

    return result;
}
