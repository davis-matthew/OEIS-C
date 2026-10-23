/*
 * A043391  Numbers having three 5's in base 6.
 * https://oeis.org/A043391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th integer that contains exactly three 5’s in its
 *            base‑6 representation (unranking of length‑wise counts)
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow5(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 5;
    return r;
}

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

static seq_t count_len(idx_t L)
{
    if (L < 3)
        return 0;
    if (L == 3)
        return 1;                /* only 555 */

    seq_t c1 = binomial(L - 1, 2) * pow5(L - 3);          /* leading 5 */
    seq_t c2 = 4 * binomial(L - 1, 3) * pow5(L - 4);      /* leading 1‑4 */
    return c1 + c2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset starts at 1 */

    /* Find the length L of the n‑th number. */
    idx_t cumulative = 0;
    idx_t L = 3;
    while (1)
    {
        idx_t cnt = (idx_t)count_len(L);
        if (cumulative + cnt >= n)
            break;
        cumulative += cnt;
        ++L;
    }

    idx_t rank = n - cumulative;   /* 1‑based rank within numbers of length L */

    seq_t value = 0;
    idx_t rem_fives = 3;

    for (idx_t pos = 0; pos < L; ++pos)
    {
        for (int d = (pos == 0 ? 1 : 0); d <= 5; ++d)
        {
            idx_t tmp_fives = rem_fives - (d == 5);
            idx_t remaining = L - pos - 1;
            if (tmp_fives > remaining)
                continue;

            seq_t ways;
            if (tmp_fives == 0)
                ways = pow5(remaining);
            else
                ways = binomial(remaining, tmp_fives) * pow5(remaining - tmp_fives);

            idx_t ways_i = (idx_t)ways;
            if (rank > ways_i)
            {
                rank -= ways_i;
                continue;
            }

            value = value * 6 + (seq_t)d;
            rem_fives = tmp_fives;
            break; /* move to next position */
        }
    }

    return value;
}
