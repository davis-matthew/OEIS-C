/*
 * A061209  Numbers which are the cubes of their digit sum.
 * https://oeis.org/A061209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A061209):
   a(n) = number such that digit_sum(a(n))^3 = a(n); a(1) = 0.
   The sequence is finite: the largest term is 19683. */

/* The function builds the sequence lazily the first time it is called
   and caches the found terms. Subsequent calls use the cache. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* static cache for the first 7 terms */
    static seq_t terms[10] = {0};
    static idx_t count = 0;      /* number of terms cached so far */
    static idx_t limit = 7;      /* known number of terms in the sequence */

    /* Build the cache on first call */
    if (count == 0) {
        idx_t m = 0;            /* candidate integer to test */
        while (count < limit) {
            uint64_t tmp = (uint64_t)m;
            uint64_t sum = 0;
            /* handle m = 0 separately */
            if (tmp == 0) {
                sum = 0;
            } else {
                while (tmp) {
                    sum += tmp % 10;
                    tmp /= 10;
                }
            }
            /* compare m with cube of its digit sum */
            seq_t cube = (seq_t)sum * sum * sum;
            if ((seq_t)m == cube) {
                terms[count++] = (seq_t)m;
            }
            m++;
        }
    }

    /* return value if it exists, otherwise 0 */
    if (n <= count && n >= 1) {
        return terms[n - 1];
    } else {
        return (seq_t)0;
    }
}
