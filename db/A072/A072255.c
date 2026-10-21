/*
 * A072255  Number of ways to partition {1,2,...,n} into arithmetic
 *          progressions, where in each partition all the progressions have
 *          the same common difference and have lengths >= 2.
 * https://oeis.org/A072255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ∑_{d=1}^{⌊n/2⌋} F(k)^r · F(k−1)^(d−r) where k = ⌊n/d⌋ and
 *            r = n mod d. F denotes the Fibonacci numbers with F(0)=0, F(1)=1
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Base cases from the sequence definition */
    if (n == 0) return 1;
    if (n == 1) return 0;

    seq_t total = 0;
    idx_t maxd = n / 2;

    for (idx_t d = 1; d <= maxd; ++d) {
        idx_t k = n / d;
        idx_t r = n % d;

        /* Compute Fibonacci numbers needed */
        seq_t fk        = fib(k);         /* F(k)   */
        seq_t fk_minus1 = fib(k - 1);     /* F(k-1) */

        /* Contributions from this common difference */
        seq_t part1 = pow_seq_local(fk,        r);
        seq_t part2 = pow_seq_local(fk_minus1, d - r);

        total += part1 * part2;
    }

    return total;
}
