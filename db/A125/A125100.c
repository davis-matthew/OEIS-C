/*
 * A125100  Triangle read by rows: T(n,k) = Fibonacci(k+1)*binomial(n,k) +
 *          (k+1)*binomial(n,k+1) (0 <= k <= n).
 * https://oeis.org/A125100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique row index such that r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 . Let k = n - r(r+1)/2 (0 ≤ k ≤ r). Then a(n) =
 *            Fibonacci(k+1) * C(r,k) + (k+1) * C(r,k+1). (C(r,k+1)=0 when
 *            k=r.)
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"

/* Find the row containing the n-th element (0‑based). */
static idx_t _row_of_index(idx_t n)
{
    idx_t low = 0, high = n;          /* high inclusive, low ≤ high */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* cumulative terms up to row mid: (mid+1)(mid+2)/2 */
        __uint128_t cum = (__uint128_t)(mid + 1) * (mid + 2) / 2;
        if (cum > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;               /* smallest r with (r+1)(r+2)/2 > n */
}

seq_t a(idx_t n)
{
    /* Locate row r and column k */
    idx_t r = _row_of_index(n);
    __uint128_t cum_prev = (__uint128_t)r * (r + 1) / 2; /* terms before row r */
    idx_t k = n - (idx_t)cum_prev; /* 0‑based column within row r */

    /* Compute the value using the triangle formula */
    seq_t fib_k1 = fib(k + 1);
    seq_t binom_rk = binomial(r, k);
    seq_t part = fib_k1 * binom_rk;

    if (k + 1 <= r) {
        seq_t binom_rk1 = binomial(r, k + 1);
        part += ((seq_t)(k + 1)) * binom_rk1;
    }
    return part;
}
