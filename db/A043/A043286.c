/*
 * A043286  Maximal run length in base-12 representation of n.
 * https://oeis.org/A043286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum run length of identical digits in base‑12
 *            representation of n
 * Range:     u8 n <= 9430, u16 n <= 9430, u32 n <= 9430, u64 n <= 9430,
 *            u128 n <= 9430, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, but safeguard */
        return 0;

    /* First digit */
    unsigned int prev = (unsigned int)(n % 12);
    n /= 12;
    unsigned int cur_run = 1, best_run = 1;

    /* Process remaining digits (least significant to most) */
    while (n > 0) {
        unsigned int digit = (unsigned int)(n % 12);
        n /= 12;
        if (digit == prev) {
            ++cur_run;
        } else {
            cur_run = 1;
        }
        if (cur_run > best_run)
            best_run = cur_run;
        prev = digit;
    }
    return (seq_t)best_run;
}
