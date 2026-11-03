/*
 * A234581  Let S_n = 0 followed by base-6 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth digit of the infinite string formed by
 *            concatenating S_k = '0' + reverse(base-6(k)) for k = 0,1,2,3,
 * Range:     u8 n <= 10001, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 121 terms through n = 9469 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline size_t base6_len(idx_t x)
{
    size_t len = 0;
    do {
        ++len;
        x /= 6;
    } while (x > 0);
    return len;
}

seq_t a(idx_t n)
{
    /* Iterate over the S_k blocks until we reach the block that contains
       the nth digit. */
    idx_t pos = 0;               /* number of digits already produced */
    for (idx_t k = 0;; ++k) {
        size_t block_len = base6_len(k) + 1;   /* len of S_k */
        if (pos + (idx_t)block_len > n) {
            size_t offset = (size_t)(n - pos);  /* 0‑based offset inside S_k */
            if (offset == 0)
                return (seq_t)0;         /* the leading '0' of S_k */
            /* The digit at offset has to be the (offset-1)th least
               significant base-6 digit of k. */
            idx_t tmp = k;
            for (size_t i = 1; i < offset; ++i)
                tmp /= 6;
            return (seq_t)(tmp % 6);
        }
        pos += (idx_t)block_len;
    }
}
