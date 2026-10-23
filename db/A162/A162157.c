/*
 * A162157  Number of square numbers from 1 to n^2 where all decimal digits
 *          are at most the corresponding digit of n^2.
 * https://oeis.org/A162157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: count squares m^2 <= n^2 such that for each decimal digit position i,
   digit(m^2, i) <= digit(n^2, i)  (digits compared from the least significant side). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* compute n^2 */
    seq_t n2 = (seq_t)n * (seq_t)n;
    /* extract decimal digits of n^2 into an array */
    unsigned char dig[40];          /* 128‑bit numbers have at most 39 decimal digits */
    idx_t dcnt = 0;
    seq_t tmp = n2;
    /* at least one digit for n2>0 */
    do {
        dig[dcnt++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp != 0);

    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t m2 = (seq_t)m * (seq_t)m;   /* square of m */
        seq_t t   = m2;
        int ok    = 1;                    /* assume valid until proven otherwise */
        for (idx_t i = 0; i < dcnt; ++i) {
            if ((t % 10) > dig[i]) {      /* digit exceeds corresponding digit of n^2 */
                ok = 0;
                break;
            }
            t /= 10;
        }
        if (ok)
            ++count;
    }
    return count;
}
