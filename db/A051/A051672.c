/*
 * A051672  Triangle of up-down sums of k-th powers:
 *          a(n,k)=sum(i^k,i=1..n)+sum((n-i)^k,i=1..n-1), n,k>0.
 * https://oeis.org/A051672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A051672 – Triangle of up‑down sums of k‑th powers
    Return the n‑th term of the flattened triangle:
        value = 2 * Σ_{t=1}^i t^k  -  i^k
    where
        r   = smallest integer with r(r+1)/2 ≥ n          (row number)
        s   = n - r(r-1)/2                                 (position in row)
        i   = r - s + 1      // row index used in a(i,k)
        k   = s              // power index
    The sequence is defined for n ≥ 1.                       */

#include <math.h>
#include <stdint.h>

/* fast exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* for safety – offset is 1 */

    /* Find row r: smallest r with r(r+1)/2 ≥ n */
    idx_t r = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    while ( (__int128)r * (r + 1) / 2 < n ) ++r;
    while ( r > 0 && (__int128)(r - 1) * r / 2 >= n ) --r;

    /* Position s inside row r (1‑based) */
    idx_t prev = r * (r - 1) / 2;   /* T(r-1) */
    idx_t s = n - prev;            /* 1 ≤ s ≤ r */

    idx_t i = r - s + 1;   /* row index in a(i,k) */
    idx_t k = s;           /* power index in a(i,k) */

    /* Compute S(i,k) = Σ_{t=1}^i t^k */
    seq_t sum = 0;
    for (idx_t t = 1; t <= i; ++t) {
        seq_t p = pow_seq_t((seq_t)t, k);
        sum += p;
    }

    /* Compute i^k */
    seq_t i_pow_k = pow_seq_t((seq_t)i, k);

    /* Result: 2*sum - i^k */
    seq_t result = sum + sum;   /* 2*sum */
    result -= i_pow_k;
    return result;
}
