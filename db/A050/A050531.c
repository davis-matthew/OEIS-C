/*
 * A050531  Number of multigraphs with loops on 3 nodes with n edges.
 * https://oeis.org/A050531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (binomial(n+5,5) + B + C)/6 * where * B =
 *            (n+2)*(n+3)*(n+4)/8 if n even, * (n+1)*(n+3)*(n+5)/8 if n odd, *
 *            C = 2*((n/3)+1) if n divisible by 3, * 0 otherwise * This
 *            matches the OEIS entry A050531
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 189, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // A = binomial(n+5, 5)
    seq_t A = binomial(n + 5, (idx_t)5);

    // B
    seq_t B;
    if (n & 1) { // odd
        seq_t prod = (seq_t)(n + 1) * (n + 3) * (n + 5);
        B = prod / 8;
    } else { // even
        seq_t prod = (seq_t)(n + 2) * (n + 3) * (n + 4);
        B = prod / 8;
    }

    // C
    seq_t C;
    if ((n % 3) == 0) {
        C = ((seq_t)(n / 3) + 1) * 2;
    } else {
        C = 0;
    }

    // Final result: (A + B + C) / 6
    seq_t sum = A + B + C;
    return sum / 6;
}
