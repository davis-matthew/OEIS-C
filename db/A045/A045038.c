/*
 * A045038  Numbers whose base-4 representation contains no 0's and exactly
 *          one 2.
 * https://oeis.org/A045038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1793, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045038 – Numbers whose base‑4 representation contains no 0’s
   and exactly one digit “2”.
   Algorithm:  construct the base‑4 representation directly.
   For a given index n (offset 1), determine the length L of the
   representation then choose the digits lexicographically.  No search
   or sieve is required. */

#include <stdbool.h>

/* helper: power of two for arbitrary seq_t */
static inline seq_t pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r <<= 1;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* 0 has no defined term */

    seq_t target = (seq_t)n;              /* convert index to seq_t */
    seq_t pref   = 0;                     /* sum of counts for shorter lengths */
    idx_t  L     = 0;                     /* length of the representation */

    /* find the minimal length L such that pref + C(L) >= target   */
    for (idx_t l = 1; ; ++l) {
        seq_t count = (l == 1) ? (seq_t)1
                               : (seq_t)l * pow2(l - 1);
        if (pref + count >= target) {
            L = l;
            break;
        }
        pref += count;
    }

    seq_t idx = target - pref - 1;        /* index within the block of length L */
    bool   used2 = false;                 /* have we placed the single '2' yet? */
    seq_t  value = 0;

    /* construct digits from most significant to least significant */
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem_len = L - pos - 1;      /* positions still to fill */
        int chosen = 0;

        /* try digits 1,2,3 in order */
        for (int d = 1; d <= 3; ++d) {
            seq_t cnt = 0;

            if (d == 2) {
                if (used2) cnt = 0;
                else       cnt = pow2(rem_len);     /* no more 2 after this place */
            }
            else { /* d is 1 or 3 */
                if (used2) {
                    cnt = pow2(rem_len);           /* remaining digits must avoid 2 */
                } else {
                    if (rem_len == 0) cnt = 0;     /* cannot place 2 later */
                    else          cnt = rem_len * pow2(rem_len - 1);
                }
            }

            if (idx < cnt) {
                chosen = d;
                break;
            } else {
                idx -= cnt;
            }
        }

        if (chosen == 2) used2 = true;
        value = value * 4 + (seq_t)chosen;
    }

    return value;
}
