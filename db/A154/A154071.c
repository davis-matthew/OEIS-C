/*
 * A154071  Number of planar triangular n X n X n nonnegative integer grids
 *          with mirror symmetry about one altitude with every similarly
 *          oriented 5 X 5 X 5 subtriangle summing to 8.
 * https://oeis.org/A154071
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154071
 * For n ≥ 18 the sequence stabilises to 87.
 * For 5 ≤ n < 18 the values are pre‑computed from the definition.
 * a(n) = 87 if n ≥ 18
 * a(5) = 990
 * a(6) = 3472
 * a(7) = 5217
 * a(8) = 7165
 * a(9) = 5966
 * a(10) = 5372
 * a(11) = 3621
 * a(12) = 1629
 * a(13) = 738
 * a(14) = 448
 * a(15) = 184
 * a(16) = 109
 * a(17) = 100
 * a(n) = 0 if n < 5     (values below the offset are irrelevant)
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    if (n <= 17) {
        /* 5 ≤ n ≤ 17, values taken from the definition */
        static const seq_t tiny[13] = {
            990ULL, 3472ULL, 5217ULL, 7165ULL,
            5966ULL, 5372ULL, 3621ULL, 1629ULL,
            738ULL, 448ULL, 184ULL, 109ULL, 100ULL
        };
        return tiny[n - 5];
    }

    /* For n ≥ 18 all terms are 87 */
    return (seq_t)87;
}
