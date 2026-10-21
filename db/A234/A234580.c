/*
 * A234580  Let S_n = 0 followed by base-5 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th digit in the concatenation of strings S_k = 0
 *            followed by the base‑5 expansion of k, reversed
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t k = 0;
    for (;;) {
        /* number of base‑5 digits of k */
        idx_t t = k;
        unsigned d = 0;
        do {
            ++d;
            t /= 5;
        } while (t != 0);
        idx_t len = 1 + d;                /* S_k has len digits */
        if (n < cumulative + len) {
            idx_t pos = n - cumulative;   /* position inside S_k (0‑based) */
            if (pos == 0)
                return (seq_t)0;          /* leading zero of S_k */
            /* digit is the (pos‑1)th least‑significant base‑5 digit of k */
            idx_t shift = pos - 1;
            idx_t digit = k;
            while (shift--) digit /= 5;
            return (seq_t)(digit % 5);
        }
        cumulative += len;
        ++k;
    }
}
