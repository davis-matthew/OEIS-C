/*
 * A045147  Numbers whose base-4 representation contains exactly two 2's and
 *          four 3's.
 * https://oeis.org/A045147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th natural number whose base‑4 representation
 *            contains exactly two '2' digits and four '3' digits. Leading
 *            zeros are not allowed. The sequence is indexed from n = 1
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   combination
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/combination.h"

/* ---------- combinatorial helpers ---------- */

/* 2^e, with e small enough that the result fits in idx_t */
static idx_t pow2_u64(idx_t e)
{
    idx_t r = 1;
    while (e--) r <<= 1;
    return r;
}

/* Count how many base‑4 numbers of total length L have exactly
   need2 digits equal to 2, need3 digits equal to 3, and the rest
   digits 0 or 1.  This counts all length‑L strings, so it is
   used for the tail of a number after the first digit. */
static idx_t count_combos(idx_t positions, idx_t need2, idx_t need3)
{
    if (need2 > positions) return 0;
    if (need3 > positions - need2) return 0;

    idx_t c1  = combination(positions,     need2);
    idx_t c2  = combination(positions - need2, need3);
    idx_t free = positions - need2 - need3;
    idx_t pow2 = pow2_u64(free);

    return c1 * c2 * pow2;
}

/* Count how many length‑L numbers have exactly two 2’s and four 3’s
   with the first digit non‑zero (1,2,or 3). */
static idx_t count_len(idx_t L)
{
    if (L < 6) return 0;                 /* need at least 6 digits */
    idx_t total = 0;
    for (int d = 1; d <= 3; ++d) {       /* first digit 1,2,or 3 */
        idx_t need2 = 2 - (d == 2);
        idx_t need3 = 4 - (d == 3);
        if (need2 > L - 1) continue;
        if (need3 > L - 1 - need2) continue;
        idx_t c1   = combination(L - 1, need2);
        idx_t c2   = combination(L - 1 - need2, need3);
        idx_t free = (L - 1) - need2 - need3;
        idx_t pow2 = pow2_u64(free);
        total += c1 * c2 * pow2;
    }
    return total;
}

/* Return the k‑th (1‑based) base‑4 number of length L that contains
   exactly two 2’s and four 3’s, with the first digit non‑zero. */
static seq_t kth_of_length(idx_t L, idx_t k)
{
    seq_t   val   = 0;
    idx_t   need2 = 2;
    idx_t   need3 = 4;

    for (idx_t pos = 0; pos < L; ++pos) {
        int start_digit = (pos == 0 ? 1 : 0);
        for (int digit = start_digit; digit <= 3; ++digit) {
            if (digit == 2 && need2 == 0) continue;
            if (digit == 3 && need3 == 0) continue;

            idx_t rem_positions = L - pos - 1;
            idx_t rem2 = need2 - (digit == 2);
            idx_t rem3 = need3 - (digit == 3);
            idx_t cnt = count_combos(rem_positions, rem2, rem3);

            if (k > cnt) {
                k -= cnt;
                continue;
            }

            val = val * 4 + (seq_t)digit;
            if (digit == 2) --need2;
            else if (digit == 3) --need3;
            break;
        }
    }
    return val;
}

/* ---------- public sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset */

    idx_t remaining = n;
    for (idx_t L = 6; ; ++L) {
        idx_t block = count_len(L);
        if (remaining <= block) {
            return kth_of_length(L, remaining);
        }
        remaining -= block;
    }
}
