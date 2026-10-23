/*
 * A183903  Number of nondecreasing arrangements of n+3 numbers in 0..8 with
 *          each number being the sum mod 9 of three others.
 * https://oeis.org/A183903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 60*n^7 + 1554*n^6 + 22680*n^5 + 203889*n^4 * +
 *            1155420*n^3 + 56636*n^2 - 28575600*n + 32296320) / 40320 * for n
 *            > 5; otherwise the values are taken from the OEIS table
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Small n: use the table from OEIS. */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)9;
            case 3: return (seq_t)242;
            case 4: return (seq_t)1866;
            case 5: return (seq_t)6503;
            default: return (seq_t)0; /* unreachable for n >= 1 */
        }
    }

    /* n > 5: evaluate the polynomial by Horner's method. */
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)1;          /* coefficient of n^8 */
    res = res * sn + (seq_t)60;        /* +60*n^7 */
    res = res * sn + (seq_t)1554;      /* +1554*n^6 */
    res = res * sn + (seq_t)22680;     /* +22680*n^5 */
    res = res * sn + (seq_t)203889;    /* +203889*n^4 */
    res = res * sn + (seq_t)1155420;   /* +1155420*n^3 */
    res = res * sn + (seq_t)56636;     /* +56636*n^2 */
    /* subtract 28575600*n */
    seq_t tmp = res * sn;
    tmp = tmp - (seq_t)28575600;
    /* add constant 32296320 and divide by 40320 */
    res = tmp * sn + (seq_t)32296320;
    return res / (seq_t)40320;
}
