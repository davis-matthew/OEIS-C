/*
 * A067369  Weight of the alternating group (A_n) in transpositions.
 * https://oeis.org/A067369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = ½·((–1)^(n+1)·(n–2)! + n·n! – abs(Stirling1(n+1, 2))).
 *            abs(Stirling1(n+1, 2)) = n!·H_n where H_n is the n‑th harmonic
 *            number
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1)=0 */
    if (n == 2) return 0;          /* a(2)=0 */

    /* factorials */
    seq_t fact_n          = factorial(n);          /* n!   */
    seq_t fact_n_minus_2  = factorial(n - 2);      /* (n‑2)! */

    /* n! · H_n  =  Σ_{k=1}^n n! / k  */
    seq_t harmonic_sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        harmonic_sum += fact_n / (seq_t)k;
    }

    /* Assemble the numerator of a(n) */
    seq_t total;
    if (n % 2 == 0) {                 /* n even → (–1)^(n+1) = –1 */
        total = fact_n * (seq_t)n - fact_n_minus_2 - harmonic_sum;
    } else {                          /* n odd  → (–1)^(n+1) = +1 */
        total = fact_n * (seq_t)n + fact_n_minus_2 - harmonic_sum;
    }

    return total / 2;                  /* division is exact */
}
