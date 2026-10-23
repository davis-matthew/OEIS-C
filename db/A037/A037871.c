/*
 * A037871  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*3^i} is base 3
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2)*∑_{i=0}^{m}|d_i−e_i|, where Sum{d_i·3^i} are the
 *            base‑3 digits of n (least significant first), and {e_i} is the
 *            multiset of {d_i} sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base‑3 digits of n */
    unsigned char digits[65];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 3);
            tmp /= 3;
        }
    }

    /* count digits 0,1,2 */
    int cnt[3] = {0, 0, 0};
    for (int i = 0; i < len; ++i) ++cnt[digits[i]];

    /* build sorted digits e_i (non‑increasing) */
    unsigned char sorted[65];
    int pos = 0;
    for (int j = 0; j < cnt[2]; ++j) sorted[pos++] = 2;
    for (int j = 0; j < cnt[1]; ++j) sorted[pos++] = 1;
    for (int j = 0; j < cnt[0]; ++j) sorted[pos++] = 0;

    /* compute sum of absolute differences */
    seq_t sum = 0;
    for (int i = 0; i < len; ++i) {
        int diff = (int)digits[i] - (int)sorted[i];
        if (diff < 0) diff = -diff;
        sum += (seq_t)diff;
    }
    return sum / 2;
}
