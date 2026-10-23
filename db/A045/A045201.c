/*
 * A045201  Numbers whose base-5 representation contains exactly three 0's and
 *          two 3's.
 * https://oeis.org/A045201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates base‑5 numbers whose representation contains exactly three 0's
   and two 3's, returning the n‑th smallest such number (OEIS A045201). */

static int
gen(idx_t L, idx_t pos, seq_t val, int zeros, int threes,
    idx_t target, idx_t *cnt, seq_t *res)
{
    if (pos == L) {
        if (zeros == 0 && threes == 0) {
            (*cnt)++;
            if ((*cnt) == target) {
                *res = val;
                return 1;
            }
        }
        return 0;
    }
    int start = (pos == 0) ? 1 : 0;
    int rem = L - (pos + 1);
    for (int d = start; d <= 4; ++d) {
        if (d == 0 && zeros == 0) continue;
        if (d == 3 && threes == 0) continue;
        int nz = zeros - (d == 0);
        int nt = threes - (d == 3);
        if (nz + nt > rem) continue;
        if (gen(L, pos + 1, val * 5 + d, nz, nt, target, cnt, res))
            return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t counter = 0;
    seq_t result = 0;
    for (idx_t L = 5; ; ++L) {
        if (gen(L, 0, 0, 3, 2, n, &counter, &result))
            return result;
    }
}
