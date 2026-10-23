/*
 * A162192  Triangle read by rows in which row n lists the divisors of n,
 *          prime(n), the consecutive composites that are greater than
 *          prime(n), and prime (n+1), but row 0 is formed by 1 and 2.
 * https://oeis.org/A162192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the flattening of rows where row r contains the divisors
 *            of r, then prime(r), then all composites strictly between
 *            prime(r) and prime(r+1), and finally prime(r+1). Row 0 is the
 *            special sequence 1,2
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/divisor_count_u64.h"  /* uint64_t divisor_count_u64(uint64_t n); */

seq_t a(idx_t n)
{
    /* Row 0: 1, 2. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Find the row that contains the index n. */
    idx_t cum = 2;      /* Total number of elements before the current row. */
    idx_t r   = 1;      /* Current row number. */

    for (;; ++r) {
        /* Size of the current row:  div_cnt  + 1 (prime) + comp_cnt + 1 (next prime) */
        idx_t dcnt = (idx_t)divisor_count_u64((uint64_t)r);
        seq_t p1   = (seq_t)get_prime(r);    /* prime(r) */
        seq_t p2   = (seq_t)get_prime(r+1);  /* prime(r+1) */
        idx_t comp_cnt = (idx_t)(p2 - p1 - 1);
        idx_t row_size = dcnt + 1 + comp_cnt + 1;  /* = dcnt + (p2-p1) + 1 */

        if (cum + row_size > n) break;   /* n lies in this row */
        cum += row_size;
    }

    /* Offset of n within the found row r. */
    idx_t dcnt = (idx_t)divisor_count_u64((uint64_t)r);
    seq_t p1   = (seq_t)get_prime(r);
    seq_t p2   = (seq_t)get_prime(r+1);
    idx_t comp_cnt = (idx_t)(p2 - p1 - 1);

    idx_t offset = n - cum;  /* 0‑based index within row r */

    /* Divisors of r (in increasing order). */
    if (offset < dcnt) {
        idx_t seen = 0;
        for (idx_t i = 1; i <= r; ++i) {
            if (r % i == 0) {
                if (seen == offset) return (seq_t)i;
                ++seen;
            }
        }
    }

    /* prime(r) */
    if (offset == dcnt) {
        return p1;
    }

    /* composites between prime(r) and prime(r+1) */
    if (offset <= dcnt + comp_cnt) {
        return p1 + (seq_t)(offset - dcnt);
    }

    /* prime(r+1) */
    return p2;
}
