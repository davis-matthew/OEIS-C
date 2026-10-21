/*
 * A054473  Number of ways of numbering the faces of a cube with nonnegative
 *          integers so that the sum of the 6 numbers is n.
 * https://oeis.org/A054473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (Burnside's lemma):
 * a(n) = (1/24) *
 *   [ C(n+5,5)
 *     + 6 * Σ_{k=0}^{⌊n/4⌋} (n - 4k + 1)
 *     + 3 * Σ_{j=0}^{⌊n/2⌋} (n - 2j + 1)(j + 1)
 *     + 8 * ([n ≡ 0 (mod 3)] * (n/3 + 1))
 *     + 6 * ([n even] * C(n/2+2,2) ) ]
 * where [P] is 1 if P is true else 0.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t n0 = (seq_t)n;            /* use seq_t for all arithmetic */

    /* term A: C(n+5,5) by successive multiplication & division */
    seq_t A = 1;
    for (int i = 1; i <= 5; ++i)
        A = A * (n0 + 6 - i) / i;   /* after each step result is integer */

    /* term B: 6 * Σ_{k=0}^{⌊n/4⌋} (n - 4k + 1) */
    idx_t m4 = n / 4;
    seq_t B = 6 * ((seq_t)(m4 + 1) * (n0 + 1 - 2 * (seq_t)m4));

    /* term C: 3 * Σ_{j=0}^{⌊n/2⌋} (n - 2j + 1)(j + 1) */
    idx_t p2 = n / 2;
    seq_t p0 = (seq_t)p2;
    seq_t C = ((p0 + 1) * (p0 + 2) * (3 * n0 + 3 - 4 * p0)) / 2;

    /* term D: 8 * (if n % 3 == 0 then (n/3 + 1) else 0) */
    seq_t D = 0;
    if (n % 3 == 0)
        D = 8 * ((seq_t)(n / 3) + 1);

    /* term E: 6 * (if n even then C(n/2+2,2) else 0) */
    seq_t E = 0;
    if ((n & 1ULL) == 0) {
        seq_t m2 = (seq_t)(n / 2);
        E = 6 * ((m2 + 2) * (m2 + 1) / 2);
    }

    seq_t sum = A + B + C + D + E;
    /* division by 24 is exact */
    return sum / 24;
}
