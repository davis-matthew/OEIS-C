/*
 * A018277  Divisors of 88.
 * https://oeis.org/A018277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018277: the divisors of 88 in increasing order. */
/* a(n) is the nth smallest positive divisor of 88, for n ≥ 1. */

seq_t a(idx_t n)
{
    /* Any n < 1 or n > 8 is out of range for the sequence. */
    if (n < 1 || n > 8)
        return (seq_t)0;

    /* One-time initialization of the 8 divisors of 88:
           2^3 * 11^1  →  divisors are {1, 2, 4, 8, 11, 22, 44, 88}   */
    static seq_t divs[8];
    static int initialized = 0;   /* flag to avoid recomputation */

    if (!initialized) {
        idx_t pos = 0;
        /* generate all combinations of 2^i * 11^j with i=0..3, j=0..1 */
        for (idx_t i = 0; i <= 3; ++i) {
            seq_t pow2 = ((seq_t)1) << i;          /* 2^i */
            for (idx_t j = 0; j <= 1; ++j) {
                seq_t pow11 = (j == 0) ? ((seq_t)1) : ((seq_t)11);
                divs[pos++] = pow2 * pow11;
            }
        }
        /* sort the 8 values in ascending order (simple insertion sort) */
        for (idx_t i = 0; i < 8 - 1; ++i) {
            for (idx_t j = i + 1; j < 8; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i]  = divs[j];
                    divs[j]  = tmp;
                }
            }
        }
        initialized = 1;
    }
    return divs[n - 1];
}
