/*
 * A025731  Index of 8^n within the sequence of the numbers of the form
 *          7^i*8^j.
 * https://oeis.org/A025731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + \sum_{k=1}^{n} \lfloor k*c \rfloor where c =
 *            log_7(8) = ln(8)/ln(7)
 * Range:     u8 n <= 20, u16 n <= 349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast path for the first term */
    if (n == 0)
        return (seq_t)1;

    /* Scaling factor 2^52 allows exact floor calculation using integer math. */
    static const unsigned long long SCALE = 1ULL << 52; /* 2^52 */

    /* Rounded numerator N ≈ c * SCALE, computed once. */
    static unsigned long long N = 0;
    if (N == 0) {
        long double r = logl(8.0L) / logl(7.0L);               /* high‑precision c */
        N = (unsigned long long)(r * (long double)SCALE + 0.5L);
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute floor(k*c) exactly: floor(k * N / SCALE)  */
        unsigned __int128 prod = (unsigned __int128)k * N;   /* product fits in 128 bits */
        seq_t fk = (seq_t)(prod >> 52);                     /* divide by SCALE */
        sum += fk;
    }

    return (seq_t)1 + (seq_t)n + sum;
}
