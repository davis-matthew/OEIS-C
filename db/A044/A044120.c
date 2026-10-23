/*
 * A044120  Numbers n such that string 3,0 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044120: numbers n such that the base‑6 representation of n
   contains the substring "30" but that of n-1 does not.  The function
   computes values by sequential search, caching results. */

#include <stdlib.h>
#if defined(__cplusplus)
#  include <vector>
#endif

/* Return 1 if the base‑6 representation of v contains the contiguous
   digits "3" followed by "0", and v has at least two digits. */
static int has30seq(seq_t v) {
    if (v < (seq_t)6)                      /* fewer than two digits */
        return 0;
    seq_t prev = v % (seq_t)6;              /* least significant digit */
    v /= (seq_t)6;
    while (v > (seq_t)0) {
        seq_t cur = v % (seq_t)6;           /* next higher digit */
        if (cur == (seq_t)3 && prev == (seq_t)0)
            return 1;
        prev = cur;
        v /= (seq_t)6;
    }
    return 0;
}

seq_t a(idx_t n) {                      /* n >= 1 */
#if defined(__cplusplus)
    static std::vector<seq_t> seq;
    static seq_t cand = (seq_t)1;
    while (seq.size() < n) {
        if (has30seq(cand) && !has30seq(cand - (seq_t)1))
            seq.push_back(cand);
        ++cand;
    }
    return seq[n - 1];
#else
    static seq_t *arr = NULL;
    static size_t len  = 0;
    static size_t cap  = 0;
    static seq_t cand = (seq_t)1;
    while (len < n) {
        if (has30seq(cand) && !has30seq(cand - (seq_t)1)) {
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t*)realloc(arr, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);
                arr  = tmp;
                cap  = new_cap;
            }
            arr[len++] = cand;
        }
        ++cand;
    }
    return arr[n - 1];
#endif
}
