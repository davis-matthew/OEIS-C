/*
 * A139156  a(n) = (n!+9)/9.
 * https://oeis.org/A139156
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + 9)/9
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Eulerian formula simplified – compute n!/9 + 1.
       For n < 6 the division by 9 is not exact; the function
       is defined only for n ≥ 6, but we return 0 for smaller n
       to keep the function well-typed. */
    if (n < 6u) {
        /* Undefined for n < 6 – return 0. */
        return (seq_t)0;
    }

    seq_t prod = (seq_t)1;
    /* For n ≥ 6, n! contains at least 9 = 3².
       Remove two factors of 3 during the accumulation to
       keep intermediate values small. */
    int need = 2;                 /* number of 3‑factors still to remove */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t val = (seq_t)i;
        while (need > 0 && val % (seq_t)3 == (seq_t)0) {
            val /= (seq_t)3;
            need--;
        }
        prod *= val;
    }

    /* At this point prod == n! / 9.  The result is prod + 1. */
    return prod + (seq_t)1;
}
