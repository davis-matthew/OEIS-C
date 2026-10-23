/*
 * A027609  Least non-partition into positive n-th powers.
 * https://oeis.org/A027609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 1 + Σ_{k=0}^{2^n} ⌊(2^n * k) / 3^n⌋ (partition
 *            count of 4^n into n-th powers minus 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* floor_sum: compute Σ_{i=0}^{n-1} ⌊(a*i+b)/m⌋  for arbitrary-precision integers */
static seq_t floor_sum(seq_t n, seq_t m, seq_t a, seq_t b)
{
    seq_t ans = 0;
    while (1) {
        /* a >= m */
        if (a >= m) {
            seq_t q = a / m;
            /* (n-1)*n/2 * q */
            seq_t temp = (n - 1) * n;
            temp = temp / 2;
            ans += temp * q;
            a = a % m;
        }
        /* b >= m */
        if (b >= m) {
            seq_t q = b / m;
            ans += n * q;
            b = b % m;
        }
        /* y = a*n + b */
        seq_t y = a * n + b;
        if (y < m) break;
        seq_t n_new = y / m;
        seq_t b_new = y % m;
        /* swap a and m */
        seq_t a_old = a;
        a = m;
        m = a_old;
        /* update n and b */
        n = n_new;
        b = b_new;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute 2^n and 3^n */
    seq_t two_n = seq_pow(2, n);
    seq_t three_n = seq_pow(3, n);

    /* t = 2^n */
    seq_t t = two_n;

    /* sum over k=0..t of floor((2^n * k)/3^n) */
    seq_t sum_floor = floor_sum(t + 1, three_n, two_n, (seq_t)0);

    /* a(n) = t + 1 + sum_floor = 2^n + 1 + sum_floor */
    seq_t result = t + 1 + sum_floor;
    return result;
}
