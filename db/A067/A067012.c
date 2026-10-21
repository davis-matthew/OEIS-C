/*
 * A067012  Absolute composites: every permutation of digits (dropping any
 *          leading zeros) is a composite number.
 * https://oeis.org/A067012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 4776, u32 n <= 4907, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: test integers >=4 incrementally; an absolute composite is
 * a composite integer whose every permutation of decimal digits (with any
 * leading zeros dropped) is also composite.  The function returns the
 * nth such integer (n>=1). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Back‑tracking over all distinct permutations of the multiset whose
 * digit counts are given in cnt[10].  The current number being built is
 * stored in cur.  If a permutation yields a value that is <4 or prime,
 * *bad_found is set. */
static void check_permutations(int remaining, int cnt[10], seq_t cur,
                               bool *bad_found)
{
    if (*bad_found) return;
    if (remaining == 0) {
        if (cur < 4 || is_prime_big(cur))
            *bad_found = true;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 0) continue;
        --cnt[d];
        check_permutations(remaining - 1, cnt, cur * 10 + d, bad_found);
        ++cnt[d];
        if (*bad_found) return;
    }
}

/* Return true iff x is an absolute composite. */
static bool is_absolute_composite(seq_t x)
{
    if (x < 4 || is_prime_big(x))
        return false;                     /* x itself must be composite */

    int cnt[10] = {0};
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        int d = (int)(t % 10);             /* convert to int for array index */
        ++cnt[d];
        t /= 10;
        ++len;
    }

    bool bad = false;
    check_permutations(len, cnt, 0, &bad);
    return !bad;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t last_checked = 3;         /* start at 4 */

    while (len < n) {
        seq_t val = ++last_checked;
        if (val < 4 || is_prime_big(val))
            continue;
        if (is_absolute_composite(val)) {
            if (len == cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = val;
        }
    }
    return list[n - 1];
}
