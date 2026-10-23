/*
 * A175968  Complement of A175967(n), where A175967(n) = the lexicographically
 *          earliest sequence with first differences as increasing sequence of
 *          nonprimes A018252(n).
 * https://oeis.org/A175968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that does not belong to the
 *            sequence A175967, whose first differences are the increasing
 *            sequence of nonprime numbers (A018252). A175967 starts with a(1)
 *            = 1; a(k) = 1 + Σ_{j=1}^{k-1} A018252(j)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_prime_idx(idx_t x) {
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (idx_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

static inline seq_t next_nonprime(idx_t *cand) {
    while (is_prime_idx(*cand)) (*cand)++;
    seq_t d = (seq_t)(*cand)++;
    return d;
}

seq_t a(idx_t n) {
    /* seq(1) = 1 ; differences are nonprime numbers starting with 1 */
    if (n == 0) return 0;   /* undefined for n=0, but guard */

    idx_t cand = 1;          /* next candidate for a nonprime difference */
    seq_t next_seq_val = 1;  /* current value of the A175967 sequence
                                (starts with a(1)=1) */
    idx_t missing_cnt = 0;   /* how many missing numbers we have seen */

    for (idx_t m = 2; ; ++m) {
        /* Move next_seq_val forward until it reaches or passes m */
        while (next_seq_val < (seq_t)m) {
            seq_t diff = next_nonprime(&cand);
            next_seq_val += diff;
        }
        if (next_seq_val == (seq_t)m) {
            /* m is in A175967, skip it */
            continue;
        } else {
            /* m is missing */
            ++missing_cnt;
            if (missing_cnt == n) {
                return (seq_t)m;
            }
        }
    }
}
