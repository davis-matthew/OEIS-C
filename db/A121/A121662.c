/*
 * A121662  Triangle read by rows: T(i,j) for the recurrence T(i,j) =
 *          (T(i-1,j) + 1)*i.
 * https://oeis.org/A121662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=1}^{i-j+1} i!/(i-k)!, where i is the smallest
 *            integer * with i*(i+1)/2 ≥ n and j = n - i*(i-1)/2. The triangle
 *            is read row by row * starting with i=1, j=1
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any positive seq_t x. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq <= x)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset is 1 */

    seq_t N = (seq_t)n;
    seq_t temp = 8 * N + 1;
    seq_t s = isqrt_seq(temp);

    seq_t i = (s - 1) / 2;
    /* correct i if the ceil was not exact */
    while ((i * (i + 1)) / 2 < N)  ++i;

    seq_t j = N - ((i * (i - 1)) / 2);
    seq_t len = i - j + 1;

    seq_t sum = 0;
    seq_t cur = 1;
    for (idx_t m = 1; m <= (idx_t)len; ++m)
    {
        cur *= (i - (seq_t)m + 1);
        sum += cur;
    }
    return sum;
}
