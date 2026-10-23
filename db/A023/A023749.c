/*
 * A023749  Plaindromes: numbers whose digits in base 7 are in nondecreasing
 *          order.
 * https://oeis.org/A023749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 895, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose digits in base 7 are nondecreasing.
   The nth term is constructed by counting nondecreasing digit
   sequences and generating the nth one in ascending order. */

static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;        /* a(1) = 0 */

    idx_t idx = n - 2;            /* index among positive numbers, 0‑based */
    idx_t cumulative = 0;
    idx_t len = 1;

    /* find the length of the base‑7 representation */
    while (1) {
        seq_t count_len = _binom(len + 5, 5);     /* C(len+5,5) */
        if ((seq_t)idx < (seq_t)cumulative + count_len)
            break;
        cumulative += (idx_t)count_len;           /* safe: count_len <= idx-cumulative */
        ++len;
    }

    idx -= cumulative;          /* offset within the current block */

    seq_t val = 0;
    idx_t r = len;
    int min_d = 1;

    /* unrank the sequence of digits (nondecreasing, digits 1..6) */
    while (r) {
        int chosen = -1;
        for (int d = min_d; d <= 6; ++d) {
            idx_t m   = 7 - d;               /* number of allowed digits */
            idx_t t   = r + m - 2;           /* formula: C(r + m - 2, m - 1) */
            idx_t k   = m - 1;
            seq_t ways = _binom(t, k);

            if ((seq_t)idx < ways) {          /* choose this digit */
                chosen = d;
                break;
            } else {                         /* skip these ways */
                idx -= (idx_t)ways;
            }
        }
        val = val * 7 + (seq_t)chosen;
        --r;
        min_d = chosen;
    }

    return val;
}
