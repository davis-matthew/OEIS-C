/*
 * A088984  a(n) = (n!!)^2 - n!.
 * https://oeis.org/A088984
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!!)² – n! (offset 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* below the offset – undefined behavior */
    seq_t fact = 1;
    seq_t df   = 1;

    /* factorial */
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* double factorial */
    if ((n & 1u) == 0) {          /* even n: 2·4·6·...·n */
        for (idx_t i = 2; i <= n; i += 2) {
            df *= (seq_t)i;
        }
    } else {                      /* odd n: 1·3·5·...·n */
        for (idx_t i = 1; i <= n; i += 2) {
            df *= (seq_t)i;
        }
    }

    seq_t result = df * df;
    result -= fact;
    return result;
}
