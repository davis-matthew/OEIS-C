/*
 * A175916  y value of the local maximum of a discrete plot of the result of
 *          concatenating the integers one to n in base x.
 * https://oeis.org/A175916
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = y value of the local maximum of the discrete plot of the result
 * of concatenating the integers one to n in base b(n), where
 *
 *      b(n) = 2                                   , n = 2
 *      b(n) = n / 2 + 2                           , n even, n > 2
 *      b(n) = (n + 3) / 2                         , n odd
 *
 * This choice of base gives the maximum value of the concatenated
 * representation over all integer bases x ≥ 2.  The function computes
 * that maximum directly.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* undefined for n < 2, return 0 */

    /* determine the optimal base */
    idx_t base;
    if (n == 2)
        base = 2;
    else if ((n & 1ULL) == 0)    /* even n */
        base = n / 2 + 2;
    else                         /* odd n */
        base = (n + 3) / 2;

    seq_t result = 0;
    seq_t base_t = (seq_t)base;      /* convert to seq_t once */

    /* current shift multiplier = base^len_i, starting with len_i = 1 */
    seq_t shift = base_t;

    /* next i where the length of the representation increases */
    idx_t next_threshold = base;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* when we hit the threshold, increment the digit length */
        if (i == next_threshold)
        {
            shift *= base_t;           /* shift becomes base^(len_i+1) */
            next_threshold *= base;    /* prepare the following threshold */
        }

        /* append the digits of i: result = result * shift + i */
        result = result * shift + (seq_t)i;
    }
    return result;
}
