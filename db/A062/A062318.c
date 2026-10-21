/*
 * A062318  Numbers of the form 3^m - 1 or 2*3^m - 1; i.e., the union of
 *          sequences A048473 and A024023.
 * https://oeis.org/A062318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n/2) - 1 if n is odd Formula: a(n) = 2*3^(n/2-1) - 1
 *            if n is even
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (n is 1‑based; offset = 1) */

static seq_t pow3(idx_t exp) {
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)3;
    while (exp) {
        if (exp & (idx_t)1) res *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;           /* not part of the sequence, safeguard */
    if (n & (idx_t)1) {                           /* odd n: a = 3^(n/2) - 1 */
        seq_t power = pow3(n >> 1);               /* n/2, integer division */
        return power - (seq_t)1;
    } else {                                      /* even n: a = 2*3^(n/2-1) - 1 */
        seq_t power = pow3((n >> 1) - (idx_t)1);  /* n/2 - 1 */
        return power * (seq_t)2 - (seq_t)1;
    }
}
