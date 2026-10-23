/*
 * A118109  Binary representation of n-th iteration of the Rule 54 elementary
 *          cellular automaton starting with a single black cell.
 * https://oeis.org/A118109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the decimal integer whose digits equal the binary *
 *            pattern produced by the n‑th iteration of Rule 54 with a single
 *            * black cell. The pattern is: * * n = 2k → "1" followed by k
 *            copies of "0001" * n = 2k+1 → "111" followed by k copies of
 *            "0111" * * Example: a(5) = "11101110111" (decimal 11...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* k = floor(n/2) */
    seq_t res;

    if ((n & 1) == 0) {        /* even n:  "1" + k × "0001"  */
        res = 1;               /* start with the leading '1' */
        for (idx_t i = 0; i < k; ++i) {
            res = res * 10;     /* append '0' */
            res = res * 10;     /* append '0' */
            res = res * 10;     /* append '0' */
            res = res * 10 + 1; /* append '1' */
        }
    } else {                   /* odd n:   "111" + k × "0111" */
        res = 111;             /* start with the leading "111" */
        for (idx_t i = 0; i < k; ++i) {
            res = res * 10;     /* append '0' */
            res = res * 10 + 1; /* append '1' */
            res = res * 10 + 1; /* append '1' */
            res = res * 10 + 1; /* append '1' */
        }
    }
    return res;
}
