/*
 * A067280  Number of terms in continued fraction for sqrt(n), excl. 2nd and
 *          higher periods.
 * https://oeis.org/A067280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003285(n)+1 = floor(sqrt(n)) + period length of sqrt(n)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Convert index n to a seq_t and compute floor(sqrt(n)) via binary search */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, div;

    while (low <= high) {
        mid = (low + high) / 2;
        if (mid == 0) {                /* avoid division by zero */
            low = 1;
            continue;
        }
        div = x / mid;                 /* x / mid is exact (integer division) */
        if (mid <= div) {              /* mid*mid <= x  <=>  mid <= x/mid */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    /* low is the smallest integer with low*low > x */
    return low - 1;                     /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;                 /* value of n as seq_t */

    /* Integer part of continued fraction */
    seq_t a0 = isqrt_seq(x);
    if (a0 * a0 == x) {                /* n is a perfect square */
        return (seq_t)1;                /* only the integer part */
    }

    /* Compute the period length of sqrt(n) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;
    seq_t period = 0;

    while (1) {
        m = d * a_k - m;
        d = (x - m * m) / d;
        a_k = (a0 + m) / d;
        period++;

        if (a_k == (seq_t)2 * a0) {    /* period ends when a_k == 2*a0 */
            break;
        }
    }

    return period + (seq_t)1;           /* include the initial term a0 */
}
