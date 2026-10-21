/*
 * A051168  Triangular array T(h,k) for 0 <= k <= h read by rows: T(h,k) =
 *          number of binary Lyndon words with k ones and h-k zeros.
 * https://oeis.org/A051168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 653, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial, idx_to_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS notation):
 *      a(n) = T(h,k) where (h,k) are the row and column of the
 *      triangular array with h≥0, 0≤k≤h, read by rows.
 *
 *      T(h,k) = (1/h) * Σ_{d | gcd(h,k)} μ(d) ⋅ C(h/d, k/d),
 *      with T(0,0)=1.
 *      Here μ(d) is the Möbius function and C is the binomial coefficient.
 *
 *  The implementation uses the provided binomial routine and performs
 *  the sum over all divisors of gcd(h,k).  It works for any seq_t
 *  and idx_t defined by the test harness.
 */

/* --------------------- utilities --------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/idx_to_row_col.h"

/* gcd for 64‑bit unsigned integers                               */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function (returns -1, 0, or 1)                           */
static int mobius(uint64_t n)
{
    if (n <= 1) return 1;   /* μ(1)=1 */
    int prime_cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
                if (exp > 1) return 0;           /* squared prime factor */
            }
            prime_cnt++;
        }
    }
    if (n > 1) prime_cnt++;   /* remaining prime factor */
    return (prime_cnt % 2 == 0) ? 1 : -1;
}

/* Compute T(h,k) by Möbius inversion                              */
static seq_t T_value(idx_t h, idx_t k)
{
    if (h == 0) return (seq_t)1;          /* T(0,0)=1 */

    uint64_t g = gcd64((uint64_t)h, (uint64_t)k);

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* iterate over all divisors d of g */
    for (uint64_t d = 1; d <= g; ++d) {
        if (g % d != 0) continue;
        int mu = mobius(d);
        if (mu == 0) continue;

        idx_t hd = h / d;
        idx_t kd = k / d;
        seq_t bin = binomial(hd, kd);

        if (mu == 1) pos_sum += bin;
        else         neg_sum += bin;   /* mu == -1 */
    }

    seq_t total = pos_sum;
    total -= neg_sum;        /* total is divisible by h */
    return total / (seq_t)h;
}

/* --------------------- public entry --------------------- */
seq_t a(idx_t n)
{
    idx_t h, k;
    idx_to_row_col(n, &h, &k);
    return T_value(h, k);
}
