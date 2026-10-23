/*
 * A023807  Xenodromes: all digits in base 13 are different.
 * https://oeis.org/A023807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑13 digits are all
 *            distinct
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexing starts at 1: a(1) = 0, a(2) = 1, … */

#include <stdbool.h>

#define B 13           /* number of digits in base‑13 (0 … 12) */

static inline idx_t perm(idx_t total, idx_t k) {
    idx_t v = 1;
    for (idx_t i = 0; i < k; ++i) {
        v *= (total - i);
    }
    return v;
}

seq_t a(idx_t n) {
    /* the first B (=13) numbers are the single‑digit values 0…12 */
    if (n <= B) {
        return (seq_t)(n - 1);
    }

    /* remove those first B numbers */
    n -= B;

    idx_t d = 2;          /* current number of base‑13 digits */
    idx_t count;

    /* find the digit length d such that n lies in its range */
    for (;;) {
        count = (B - 1) * perm(B - 1, d - 1);   /* numbers of length d */
        if (n <= count) {
            break;
        }
        n -= count;
        ++d;
    }

    /* construct the desired number digit by digit */
    bool used[B] = {0};   /* flags for digits that have been used */
    seq_t result = 0;
    idx_t used_cnt = 0;

    /* choose the leading digit (cannot be 0) */
    for (idx_t dig = 1; dig < B; ++dig) {
        idx_t cnt = perm(B - 1, d - 1);   /* permutations with this leading digit */
        if (n > cnt) {
            n -= cnt;
        } else {
            result = dig;
            used[dig] = true;
            used_cnt = 1;
            break;
        }
    }

    /* choose the remaining digits */
    for (idx_t pos = 1; pos < d; ++pos) {
        for (idx_t cand = 0; cand < B; ++cand) {
            if (!used[cand]) {
                idx_t avail = B - used_cnt;           /* unused digits before picking cand */
                idx_t remaining = d - pos - 1;        /* positions left after this one */
                idx_t cnt = perm(avail - 1, remaining);
                if (n > cnt) {
                    n -= cnt;
                } else {
                    result = result * (seq_t)B + cand;
                    used[cand] = true;
                    ++used_cnt;
                    break;
                }
            }
        }
    }

    return result;
}
