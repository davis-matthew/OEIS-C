/*
 * A040186  Continued fraction for sqrt(201).
 * https://oeis.org/A040186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = Continued fraction terms for sqrt(201)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hidden definition: a(0) = floor(sqrt(201)) = 14, followed by the periodic block  
   (5, 1, 1, 1, 2, 1, 8, 1, 2, 1, 1, 1, 5, 28) with period length 14. */
static const seq_t period[14] = {
    (seq_t)5, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)2, (seq_t)1, (seq_t)8, (seq_t)1,
    (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)5, (seq_t)28
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;
    /* (n-1) is safe because we guard n==0 above */
    idx_t idx = (n - 1) % (idx_t)(sizeof(period)/sizeof(period[0]));
    return period[idx];
}
