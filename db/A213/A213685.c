/*
 * A213685  Arises in enumerating maximal antichains of minimum size.
 * https://oeis.org/A213685
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(4) = 1; For n ≥ 5, a(n) = a(n‑1) + d(n), where d(n) = (n-2)/2
 *            if n ≡ 0 (mod 4) n/2 if n ≡ 2 (mod 4) (3*n-7)/4 if n ≡ 1 (mod 4)
 *            3*((n-1)/4) if n ≡ 3 (mod 4)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;          /* offset is 4, values below are unused */

    seq_t res = 1;         /* a(4) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t d;
        switch (i & 3) {
            case 0:                  /* i ≡ 0 (mod 4) */
                d = ((seq_t)(i - 2)) / 2;
                break;
            case 1:                  /* i ≡ 1 (mod 4) */
                d = (3 * (seq_t)i - 7) / 4;
                break;
            case 2:                  /* i ≡ 2 (mod 4) */
                d = (seq_t)i / 2;
                break;
            case 3:                  /* i ≡ 3 (mod 4) */
                d = 3 * ((seq_t)(i - 1) / 4);
                break;
        }
        res += d;
    }
    return res;
}
