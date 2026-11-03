/*
 * A132337  Sum of the integers from 1 to n, excluding the perfect sixth
 *          powers.
 * https://oeis.org/A132337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n(n+1)/2 - sum_{k=1}^{floor(n^(1/6))} k^6
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8390 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Raise a number to the sixth power (seq_t version) */
static seq_t pow6_seq(seq_t x)
{
    seq_t x2 = x * x;     /* x²    */
    seq_t x4 = x2 * x2;   /* x⁴    */
    return x4 * x2;       /* x⁶    */
}

/* Compute the sum of sixth powers up to r */
static seq_t sum_sixth_powers(idx_t r)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= r; ++i)
    {
        seq_t k = (seq_t)i;          /* k      */
        seq_t k2 = k * k;            /* k²     */
        seq_t k4 = k2 * k2;          /* k⁴     */
        seq_t k6 = k4 * k2;          /* k⁶     */
        sum += k6;
    }
    return sum;
}

/* Integer floor of the sixth root of n */
static idx_t floor_sixth_root(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound */
    while (1)
    {
        seq_t high6 = pow6_seq((seq_t)high);
        if (high6 > (seq_t)n)
            break;
        high <<= 1;
    }

    /* Binary search */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        seq_t mid6 = pow6_seq((seq_t)mid);
        if (mid6 <= (seq_t)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t total = n_seq * (n_seq + 1) / 2;          /* 1 + 2 + … + n */
    idx_t r = floor_sixth_root(n);                 /* floor(n^(1/6)) */
    seq_t subtract = sum_sixth_powers(r);           /* sum of k⁶, k=1..r */
    return total - subtract;
}
