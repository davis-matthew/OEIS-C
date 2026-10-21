/*
 * A095861  Number of primitive Pythagorean triangles of form (X,Y,Y+1) with
 *          hypotenuse Y+1 less than or equal to n.
 * https://oeis.org/A095861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor((sqrt(2*n-1) - 1)/2)
 * Range:     u8 n <= 16, u16 n <= 4670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t v)
{
    seq_t low = 0;
    seq_t high = v;
    while (low <= high)
    {
        seq_t mid = low + ((high - low) >> 1);   /* safer mid calculation */
        seq_t mid2 = mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else
        {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    /* convert index to seq_t to avoid overflow in multiplication */
    seq_t nn = (seq_t)n;
    seq_t m = ((seq_t)2) * nn - (seq_t)1;   /* 2*n - 1 */
    seq_t s = isqrt_seq(m);                 /* floor( sqrt(2*n-1) ) */
    return (s - (seq_t)1) >> 1;             /* (s-1)/2 */
}
