/*
 * A087408  Multiples of 5 with digits grouped in pairs and leading zeros
 *          omitted.
 * https://oeis.org/A087408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = decimal value of the nth pair of consecutive digits in
 *            the infinite string obtained by concatenating the decimal
 *            representations of 5·k for k = 1,2,3,…, with leading zeros
 *            omitted from each pair
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t pair_idx = 0;          /* count of pairs seen so far (1‑based) */
    idx_t k = 1;                 /* current multiple index */
    int  prev = -1;              /* previous digit, -1 when none */

    while (1)
    {
        /* current multiple of 5 */
        idx_t m = 5 * k;         /* use idx_t to avoid overflow for small seq_t */

        /* convert m to its decimal digits (left to right) */
        /* temporary buffer – maximum 20 digits for 64‑bit numbers */
        int digits[20];
        int len = 0;
        idx_t t = m;
        while (t > 0) {
            digits[len++] = t % 10;
            t /= 10;
        }
        /* process digits from most significant to least significant */
        for (int i = len - 1; i >= 0; --i) {
            int d = digits[i];
            if (prev == -1) {
                prev = d;
            } else {
                /* form the pair (prev, d) */
                seq_t val = (seq_t)prev * 10 + d;   /* leading zeros automatically omitted */
                ++pair_idx;
                if (pair_idx == n)
                    return val;
                prev = -1;
            }
        }
        ++k;      /* move to the next multiple of 5 */
    }
}
