/*
 * A174990  Partial sums of A005606.
 * https://oeis.org/A174990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = Σ_{i=0..n} A005606(i) where A005606(i) is the position
 *            (1‑26) of the first letter of the English word for i, treating 0
 *            as “zero” (z → 26)
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t first_letter_value(idx_t n)
{
    /* 1‑9   : o, t, t, f, f, s, s, e, n   */
    static const seq_t ones[10] = {0,15,20,20,6,6,19,19,5,14};

    /* 10‑19: t, e, t, t, f, f, s, s, e, n   */
    static const seq_t teens[20] = {
        0,0,0,0,0,0,0,0,0,0,
        20,5,20,20,6,6,19,19,5,14
    };

    /* 20‑90: t, t, f, f, s, s, e, n   */
    static const seq_t tens[10] = {0,0,20,20,6,6,19,19,5,14};

    if (n == 0)
        return 26;         /* “zero” → z → 26 */

    /* Find the most significant non‑zero triplet (thousands, millions…) */
    while (n >= 1000)
        n /= 1000;

    if (n < 10)
        return ones[n];

    if (n < 20)
        return teens[n];

    if (n < 100)
        return tens[n / 10];

    /* 100 ≤ n < 1000 – first word is the hundreds digit word */
    return ones[n / 100];
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += first_letter_value(i);
    return sum;
}
