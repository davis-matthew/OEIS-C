/*
 * A166976  Array of A002450 in the top row and higher-order differences in
 *          subsequent rows, read by antidiagonals.
 * https://oeis.org/A166976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (4^j - 1)/3 if s==j, 4^j * 3^(s-j-1) otherwise where s =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - s*(s+1)/2. This follows
 *            from computing the (s-j)-th forward difference of u(k) =
 *            (4^k-1)/3
 * Range:     u8 n <= 18, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast integer power for any seq_t type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    idx_t count = 0;   /* number of elements before diagonal s */
    while (1) {
        idx_t len = s + 1;               /* entries in current diagonal */
        if (count + len > n) break;
        count += len;
        ++s;
    }

    idx_t j = n - count;   /* position inside diagonal, 0 <= j <= s */
    idx_t i = s - j;       /* order of difference */

    if (i == 0) {
        seq_t pow4 = pow_seq((seq_t)4, j);
        seq_t num  = pow4 - (seq_t)1;
        return num / (seq_t)3;   /* (4^j - 1)/3   */
    } else {
        seq_t pow4 = pow_seq((seq_t)4, j);
        seq_t pow3 = pow_seq((seq_t)3, i - 1);
        return pow4 * pow3;      /* 4^j * 3^(i-1) */
    }
}
