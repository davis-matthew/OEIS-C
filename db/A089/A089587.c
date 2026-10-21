/*
 * A089587  a(n) is the smallest integer k, 0<k<n, that most often satisfies
 *          the condition: k^m > k^(m+1) (modulo n) as m varies from 1 to n-1,
 *          for n>2, with a(1)=0 and a(2)=1.
 * https://oeis.org/A089587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer 0<k<n that maximizes the number of
 *            times k^m mod n > k^(m+1) mod n for m = 1..n-1. a(1)=0, a(2)=1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used by OEIS, but defensive */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t best_k = 1;
    int best_cnt = -1;

    for (idx_t k = 1; k < n; ++k) {
        seq_t r = (seq_t)k;          /* k^1 mod n */
        int cnt = 0;
        for (idx_t m = 1; m < n; ++m) {
            seq_t next = (seq_t)((uint64_t)r * k % n);
            if (r > next) ++cnt;
            r = next;
        }
        if (cnt > best_cnt || (cnt == best_cnt && k < best_k)) {
            best_cnt = cnt;
            best_k   = (seq_t)k;
        }
    }
    return best_k;
}
