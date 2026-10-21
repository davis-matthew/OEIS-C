/*
 * A232095  Minimal number of factorials which add to 0+1+2+...+n; a(n) =
 *          A034968(A000217(n)).
 * https://oeis.org/A232095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of factorials summing to n*(n+1)/2
 * Range:     u8 n <= 19, u16 n <= 1853, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    seq_t target = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;
    if (target == (seq_t)0) return (seq_t)0;

    static seq_t fact[128];
    static size_t fact_cnt = 0;

    if (fact_cnt == 0) {
        fact[0] = (seq_t)1;          /* 1! = 1 */
        fact_cnt = 1;
    }

    /* Expand factorial table to include all factorials <= target */
    while (fact_cnt < sizeof(fact) / sizeof(fact[0])) {
        seq_t next = fact[fact_cnt - 1] * ((seq_t)fact_cnt + (seq_t)1);
        if (next <= fact[fact_cnt - 1]) break;   /* overflow protection */
        if (next > target) break;
        fact[fact_cnt++] = next;
    }

    seq_t count = 0;
    long long idx = (long long)(fact_cnt - 1);
    while (idx >= 0 && target != (seq_t)0) {
        count += target / fact[idx];
        target %= fact[idx];
        idx--;
    }
    return count;
}
