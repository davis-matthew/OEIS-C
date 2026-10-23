/*
 * A164308  Triangle read by rows, binomial distribution of the terms (1, 3,
 *          9, 27, ...).
 * https://oeis.org/A164308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = 3^{popcount( Gray( (n+1) - 2^{⌊log2(n+1)⌋} ))}
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Gray(k) = k ^ (k >> 1) */
/*  popcount(x) counts 1‑bits of x */

#include <stdint.h>

/* returns a(n) for the OEIS sequence A164308 */
seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit value, idx_t provided by the build */
    uint64_t np1 = n + 1u;                     /* m = n+1, m >= 1         */
    int r = 63 - __builtin_clzll(np1);         /* floor(log2(m))           */
    uint64_t start = ((uint64_t)1 << r) - 1u;  /* first index of this row  */
    uint64_t k = n - start;                    /* offset within the row     */

    uint64_t gray = k ^ (k >> 1);              /* Gray code of k           */
    int e = __builtin_popcountll(gray);        /* exponent of 3             */

    /* compute 3^e */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (e > 0) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
