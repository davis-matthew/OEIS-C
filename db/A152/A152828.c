/*
 * A152828  Triangle read by rows, A007318 rows repeated three times.
 * https://oeis.org/A152828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = binomial(r, k) where r is the row index such that
   n lies in the repeated block of length 3*(r+1);
   k = (n - start_of_row) % (r+1) */
#include <stdint.h>

static seq_t binomial_row(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t remaining = n;
    /* subtract whole repeated blocks until we land inside the
       current row block */
    while (remaining >= 3 * (r + 1)) {
        remaining -= 3 * (r + 1);
        ++r;
    }
    /* position inside the repeated row */
    idx_t k = remaining % (r + 1);
    return binomial_row(r, k);
}
