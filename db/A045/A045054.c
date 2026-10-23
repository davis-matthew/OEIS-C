/*
 * A045054  Numbers whose base-4 representation contains exactly three 0's and
 *          two 2's.
 * https://oeis.org/A045054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains exactly three 0's
 *            and two 2's
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

static seq_t pow2_seq(idx_t e)
{
    seq_t r = 1;
    r <<= e;
    return r;
}

static seq_t count_len(idx_t L)
{
    if (L < 5) return 0;
    return binom_seq(L - 1, 3) * binom_seq(L - 3, 2) * pow2_seq(L - 5);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t L = 5;
    seq_t cumulative = 0;
    while (1) {
        seq_t countL = count_len(L);
        if (cumulative + countL >= (seq_t)n) break;
        cumulative += countL;
        ++L;
    }
    seq_t k = (seq_t)n - cumulative;          /* 1‑based index within length L */
    idx_t zeros_rem = 3;
    idx_t twos_rem = 2;
    seq_t result = 0;

    for (idx_t pos = 1; pos <= L; ++pos) {
        idx_t rem = L - pos;
        idx_t start = 0, end = 3;
        if (pos == 1) start = 1;                /* leading digit cannot be 0 */
        for (idx_t d = start; d <= end; ++d) {
            if (d == 0 && zeros_rem == 0) continue;
            if (d == 2 && twos_rem == 0) continue;

            idx_t z2 = zeros_rem;
            idx_t t2 = twos_rem;
            if (d == 0) --z2;
            if (d == 2) --t2;

            if (z2 > rem || t2 > rem) continue;

            seq_t comb1 = binom_seq(rem, z2);
            if (comb1 == 0) continue;
            seq_t comb2 = binom_seq(rem - z2, t2);
            if (comb2 == 0) continue;

            idx_t rest = rem - z2 - t2;
            seq_t completions = comb1 * comb2 * pow2_seq(rest);

            if (k > completions) {
                k -= completions;
            } else {
                result = result * 4 + d;
                zeros_rem = z2;
                twos_rem = t2;
                break;
            }
        }
    }
    return result;
}
