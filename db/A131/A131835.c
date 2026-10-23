/*
 * A131835  Numbers starting with 1.
 * https://oeis.org/A131835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (8*10^floor(log10(9*n-8))-8)/9
 * Range:     u8 n <= 29, u16 n <= 8178, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n==0 return 0 as a defensive default. */
    if (n == 0) return 0;

    /* m = 9*n - 8;  computed in seq_t to avoid overflow for supported n. */
    seq_t m = (seq_t)9 * (seq_t)n - (seq_t)8;

    /* d = floor(log10(m)) = number of decimal digits of m minus one. */
    idx_t d = 0;
    seq_t tmp = m;
    while (tmp >= (seq_t)10) {
        tmp /= (seq_t)10;
        ++d;
    }

    /* Compute 10^d in seq_t. */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Apply the formula using only integer arithmetic. */
    seq_t term = (seq_t)n + (((seq_t)8 * pow10) - (seq_t)8) / (seq_t)9;
    return term;
}
