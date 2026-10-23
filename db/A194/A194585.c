/*
 * A194585  Starting points of stapled intervals of length 17.
 * https://oeis.org/A194585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-15031 + 10631*(-1)^n + 30030*n) / 2
 * Range:     u8 none, u16 n <= 3, u32 n <= 666, u64 n <= 666, u128 n <= 666,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = (30030*n - 25662)/2 if n is odd, else (30030*n - 4400)/2 */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is irrelevant for the tests */
    seq_t term = (seq_t)30030 * n;          /* 30030*n  */
    if (n & 1ULL) {                         /* odd n  */
        /* numerator: 30030*n - 25662  */
        seq_t num = term - (seq_t)25662;
        return num / 2;
    } else {                                /* even n */
        /* numerator: 30030*n - 4400  */
        seq_t num = term - (seq_t)4400;
        return num / 2;
    }
}
