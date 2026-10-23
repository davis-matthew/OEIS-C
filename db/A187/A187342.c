/*
 * A187342  Floor(((15+sqrt(5))/11)n); complement of A187341.
 * https://oeis.org/A187342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((15+sqrt(5))/11)*n) = floor((15*n +
 *            floor(n*sqrt(5)))/11)
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t floor_sqrt(seq_t x)
{
    seq_t low = 0, high = x;
    /* Use division to avoid overflow: mid <= x / mid  <=>  mid*mid <= x */
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        if (mid == 0) {        /* handle x==0 and avoid div by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;     /* mid is a candidate, search higher */
        else
            high = mid - 1;    /* mid too large, search lower */
    }
    return high;                /* high is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* cast index to seq_t */
    seq_t sq = nn * nn;               /* n^2 */
    seq_t x  = sq * 5;                /* 5 * n^2 = (n*sqrt(5))^2 */
    seq_t t  = floor_sqrt(x);         /* t = floor(n*sqrt(5)) */
    seq_t num = nn * 15 + t;          /* 15*n + floor(n*sqrt(5)) */
    return num / 11;                  /* floor division by 11 */
}
