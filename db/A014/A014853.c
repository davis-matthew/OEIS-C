/*
 * A014853  Numbers k that divide s(k), where s(1)=1, s(j)=6*s(j-1)+j.
 * https://oeis.org/A014853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that k | s(k), where
 *            s(1)=1 and s(k)=6*s(k-1)+k. The test reduces to (A(k)-B(k)) % k
 *            == 0, where A(k)=∑_{i=0}^{k-1} r^i, B(k)=∑_{i=0}^{k-1} (i+1)
 *            r^i, and r = 6 mod k. A(k) and B(k) are computed in O(log k)
 *            time
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: multiplication modulo a small `mod` using 128‑bit precision
   for built‑in integer types.  For arbitrary‑precision types the product
   is computed by the class itself. */
static inline seq_t mul_mod(seq_t a, seq_t b, idx_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((unsigned __int128)a * b) % mod);
#else
    return (a * b) % (seq_t)mod;
#endif
}

/* Fast exponentiation (base^exp) mod mod. */
static inline seq_t pow_mod_seq(seq_t base, idx_t exp, idx_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1U)
            result = mul_mod(result, b, mod);
        b = mul_mod(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

/* Recursive computation of A(n)=∑_{i=0}^{n-1} r^i and
   B(n)=∑_{i=0}^{n-1} (i+1) r^i, both modulo `mod`. */
typedef struct { seq_t A; seq_t B; } pairAB;

static pairAB sum_geom_weighted(idx_t n, idx_t mod, seq_t r)
{
    if (n == 0)
        return (pairAB){0, 0};
    if (n == 1)
        return (pairAB){1 % mod, 1 % mod};

    idx_t m = n >> 1;
    pairAB half = sum_geom_weighted(m, mod, r);
    seq_t pow_r_m = pow_mod_seq(r, m, mod);

    /* A(2m) = A(m) * (1 + r^m) mod mod */
    seq_t A2 = (half.A + mul_mod(pow_r_m, half.A, mod)) % mod;

    /* B(2m) = B(m) + r^m * (B(m) + m * A(m)) mod mod */
    seq_t temp = (half.B + mul_mod((seq_t)(m % mod), half.A, mod)) % mod;
    seq_t B2 = (half.B + mul_mod(pow_r_m, temp, mod)) % mod;

    if ((n & 1U) == 0)
        return (pairAB){A2, B2};

    /* For odd n = 2m + 1 */
    seq_t pow_r_2m = mul_mod(pow_r_m, pow_r_m, mod);
    seq_t A_n = (A2 + pow_r_2m) % mod;
    seq_t B_n = (B2 + mul_mod((seq_t)(n % mod), pow_r_2m, mod)) % mod;
    return (pairAB){A_n, B_n};
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    idx_t count = 0;
    for (idx_t k = 1;; ++k)
    {
        if (k == 1)
        {
            ++count;
            if (count == n) return 1;
            continue;
        }
        idx_t mod = k;
        seq_t r = (seq_t)(6 % mod);
        pairAB res = sum_geom_weighted(k, mod, r);
        seq_t s_mod = (res.A + mod - res.B) % mod;  /* (A - B) mod k */
        if (s_mod == 0)
        {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
    /* unreachable */
    return 0;
}
