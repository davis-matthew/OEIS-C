/*
 * A045238  Numbers whose base-5 representation contains exactly one 1 and two
 *          3's.
 * https://oeis.org/A045238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑5 representation contains
 *            exactly one 1 and exactly two 3s
 * Range:     u8 n <= 12, u16 n <= 7152, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive helper to enumerate numbers of a fixed length in increasing order. */
static int gen(idx_t pos, idx_t len,
               idx_t rem1, idx_t rem3,
               seq_t val,
               idx_t *cnt, idx_t target, seq_t *res)
{
    if (pos == len) {
        if (rem1 == 0 && rem3 == 0) {
            (*cnt)++;
            if (*cnt == target) {
                *res = val;
                return 1;
            }
        }
        return 0;
    }

    for (idx_t d = 0; d <= 4; ++d) {
        if (pos == 0 && d == 0) continue;   /* no leading zero */

        idx_t nrem1 = rem1;
        idx_t nrem3 = rem3;

        if (d == 1) {
            if (nrem1 == 0) continue;
            nrem1--;
        } else if (d == 3) {
            if (nrem3 == 0) continue;
            nrem3--;
        }

        seq_t next_val = val * 5 + d;
        if (gen(pos + 1, len, nrem1, nrem3,
                next_val, cnt, target, res))
            return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t count = 0;
    seq_t result = 0;

    for (idx_t len = 3; ; ++len) {
        if (gen(0, len, 1, 2, 0, &count, n, &result))
            return result;
    }
}
