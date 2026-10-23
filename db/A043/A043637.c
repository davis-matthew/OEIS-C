/*
 * A043637  Numbers whose base-9 representation has exactly 8 runs.
 * https://oeis.org/A043637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 8‑digit base‑9 numbers with exactly 8 runs (no two consecutive digits equal). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */
    idx_t offset = (idx_t)n - 1;        /* 0‑based index among the valid numbers */

    seq_t val = 0;
    int prev = -1;                      /* previous digit, -1 for no previous */

    for (int pos = 0; pos < 8; ++pos) {
        int remaining = 7 - pos;        /* number of positions after this one */

        /* pow = 8^remaining: number of completions for each candidate digit */
        idx_t pow = 1;
        for (int i = 0; i < remaining; ++i) pow *= 8;

        for (int d = 0; d <= 8; ++d) {
            if (pos == 0 && d == 0) continue;        /* leading digit non‑zero */
            if (pos > 0 && d == prev) continue;      /* no equal adjacent digits */

            if (offset < pow) {                     /* choose this digit */
                prev = d;
                val = val * 9 + (seq_t)d;
                break;
            }
            offset -= pow;                           /* skip this block of numbers */
        }
    }

    return val;
}
