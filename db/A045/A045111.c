/*
 * A045111  Numbers whose base-4 representation contains no 1's and exactly
 *          two 3's.
 * https://oeis.org/A045111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑4 representation contains no 1's and exactly
 *            two 3's
 * Range:     u8 n <= 24, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t e)
{
    seq_t r = 1;
    while (e--) r <<= 1;
    return r;
}

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - i + 1) / i;
    return r;
}

static seq_t count_suffix(idx_t R, idx_t rem3)
{
    if (rem3 > R || rem3 < 0) return 0;
    return binomial(R, rem3) * power_of_two(R - rem3);
}

static seq_t count_length(idx_t L)
{
    if (L < 2) return 0;
    seq_t term1 = (seq_t)(L - 1) * power_of_two(L - 2);   /* leading 3 case */
    seq_t term2 = 0;
    if (L >= 3)
        term2 = binomial(L - 1, 2) * power_of_two(L - 3);  /* leading 2 case */
    return term1 + term2;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range */
    /* Find the base‑4 length L that contains the nth number */
    idx_t L = 1;
    seq_t cumulative = 0;
    for (;; ++L) {
        seq_t cnt = count_length(L);
        if (cumulative + cnt >= (seq_t)n) break;
        cumulative += cnt;
    }

    seq_t rank = (seq_t)n - cumulative;   /* 1‑based rank within this length */

    seq_t val = 0;
    idx_t rem3 = 2;                     /* remaining 3's to place */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t remaining = L - pos - 1;
        /* allowed digits in ascending order, with leading‑zero restriction */
        int digits[3];
        int dcnt;
        if (pos == 0) {
            digits[0] = 2; digits[1] = 3; dcnt = 2;
        } else {
            digits[0] = 0; digits[1] = 2; digits[2] = 3; dcnt = 3;
        }
        for (int di = 0; di < dcnt; ++di) {
            int d = digits[di];
            idx_t after = rem3 - (d == 3 ? 1 : 0);
            if (after > remaining) continue;  /* impossible to finish */

            seq_t possible = count_suffix(remaining, after);
            if (possible < rank) {
                rank -= possible;
                continue;
            }
            /* this digit is chosen */
            val = val * 4 + (seq_t)d;
            rem3 = after;
            break;
        }
    }
    return val;
}
