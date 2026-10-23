/*
 * A047413  Numbers that are congruent to {3, 4, 6} mod 8.
 * https://oeis.org/A047413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*k - 2 if n = 3*k; a(n) = 8*k - 5 if n = 3*k - 2; a(n) =
 *            8*k - 4 if n = 3*k - 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented via case on n mod 3 */
seq_t a(idx_t n)
{
    if (!n) return 0;          /* behavior below offset does not matter */

    idx_t mod = n % 3;
    idx_t k;
    seq_t val;

    if (mod == 0) {            /* n = 3*k */
        k = n / 3;
        val = (seq_t)8 * (seq_t)k - (seq_t)2;
    } else if (mod == 1) {     /* n = 3*k - 2  → k = (n+2)/3 */
        k = (n + 2) / 3;
        val = (seq_t)8 * (seq_t)k - (seq_t)5;
    } else {                  /* mod == 2; n = 3*k - 1  → k = (n+1)/3 */
        k = (n + 1) / 3;
        val = (seq_t)8 * (seq_t)k - (seq_t)4;
    }

    return val;
}
