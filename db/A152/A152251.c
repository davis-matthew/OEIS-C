/*
 * A152251  Eigentriangle, row sums = A001519, odd-indexed Fibonacci numbers.
 * https://oeis.org/A152251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   a(n) = T[r][j] where * T[i][j] = (2^(i-j-1)) * Fib(2*j-3) if i >
 *            j, * T[i][j] = Fib(2*j-3) if i = j. * * Here Fib(k) is the
 *            Fibonacci number with Fib(0)=0, Fib(1)=1. * This is the
 *            Eigentriangle described in A152251
 * Range:     u8 n <= 44, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t r = 1;
    idx_t sum = 1;                 /* sum = r*(r+1)/2 for current r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Column index j within the row (1-based). */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t j = n - prev_sum;        /* 1 <= j <= r */

    /* Fibonacci value for this column. */
    seq_t Qj;
    if (j <= 2) {
        Qj = (seq_t)1;
    } else {
        Qj = fib((idx_t)(2 * j - 3));
    }

    if (r == j) {
        return Qj;
    } else {
        idx_t exp = r - j - 1;     /* exponent for the power of two */
        return pow2(exp) * Qj;
    }
}
