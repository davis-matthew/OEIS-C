/*
 * A045190  Numbers whose base-5 representation contains no 0's and exactly
 *          three 3's.
 * https://oeis.org/A045190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-5 representation contains no 0's and exactly
 *            three 3's. Sequence is enumerated in increasing order
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*--- Recursive enumeration of all length-L strings with exactly 3 threes
   (digits 1..4, no zero).  The strings are generated in lexicographic order,
   which corresponds to numeric order in base 5.  When the target position
   is reached the resulting value is stored in *result.  Returned value
   is TRUE when found. */
static bool enum_len_rec(idx_t depth, idx_t len, idx_t remaining_3,
                         seq_t value, idx_t target, idx_t *seen, seq_t *result) {
    if (*seen > target) return true;   /* already passed target */
    if (depth == len) {
        if (remaining_3 == 0) {
            if (*seen == target) {
                *result = value;
                return true;
            }
            ++(*seen);
        }
        return false;
    }
    /* iterate over digits 1..4 in ascending order */
    for (int d = 1; d <= 4; ++d) {
        if (d == 3 && remaining_3 == 0) continue;
        if (enum_len_rec(depth + 1, len, remaining_3 - (d == 3),
                         value * 5 + d, target, seen, result)) {
            return true;
        }
    }
    return false;
}

/*--- Helper: binomial coefficient C(n,k) for small n (idx_t is unsigned) */
static idx_t comb(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/*--- Helper: power of 3: 3^exp */
static idx_t pow3(idx_t exp) {
    idx_t p = 1;
    for (idx_t i = 0; i < exp; ++i) p *= 3;
    return p;
}

/* Helper to obtain the target number of length len at position pos
   (0‑based within the length‑len block). */
static seq_t generate_len(idx_t len, idx_t pos) {
    seq_t res = 0;
    idx_t seen = 0;
    enum_len_rec(0, len, 3, 0, pos, &seen, &res);
    return res;
}

/*--- Public function required by the exercise --*/
seq_t a(idx_t n) {
    if (n < 1) return 0;      /* undefined for n < 1, return 0 */

    idx_t cumulative = 0;

    for (idx_t len = 3; ; ++len) {
        idx_t count_len = comb(len, 3) * pow3(len - 3);
        if (cumulative + count_len >= n) {
            idx_t pos = n - cumulative - 1;   /* 0‑based in this block */
            return generate_len(len, pos);
        }
        cumulative += count_len;
    }
}
