/*
 * A025697  Index of 3^n within the sequence of the numbers of the form
 *          3^i*6^j.
 * https://oeis.org/A025697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} ceil(k * c), where c = log_6 3 ≈
 *            0.613147180559913
 * Range:     u8 n <= 27, u16 n <= 461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define NUM 613147180559913000ULL   /* Approximation of c * 1e18  */
#define DEN 1000000000000000000ULL  /* 1e18 */

static inline seq_t ceil_k_c(idx_t k)
{
    /* compute ceil(k * NUM / DEN) using 128‑bit arithmetic to avoid overflow   */
    unsigned __int128 prod = (unsigned __int128)k * NUM;
    return (seq_t)((prod + DEN - 1u) / DEN);
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 1; /* a(0) = 1  */
    for (idx_t k = 1; k <= n; ++k)
        sum += ceil_k_c(k);

    return sum;
}
