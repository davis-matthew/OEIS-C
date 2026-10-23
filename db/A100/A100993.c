/*
 * A100993  Indices k of Fibonacci numbers F(k) (A000045) that are divisible
 *          by k-1.
 * https://oeis.org/A100993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A069104(n) + 1
 * Range:     u8 n <= 29, u16 n <= 3319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline void fib_pair_uint64(uint64_t n, uint64_t mod,
                                   uint64_t *f, uint64_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1 % mod;        /* handle mod == 1 */
        return;
    }
    uint64_t a, b;
    fib_pair_uint64(n >> 1, mod, &a, &b);

    /* c = F(2k)   = F(k) * [2*F(k+1) – F(k)]   (mod mod) */
    uint64_t t = mod_mul_uint64(b, 2, mod);           /* t = 2*F(k+1) mod mod */
    uint64_t c = mod_mul_uint64(a, (t + mod - a) % mod, mod);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2   (mod mod) */
    uint64_t d = (mod_mul_uint64(a, a, mod) + mod_mul_uint64(b, b, mod)) % mod;

    if ((n & 1) == 0) {            /* n even  */
        *f = c;
        *f_next = d;
    } else {                       /* n odd   */
        *f = d;
        *f_next = (c + d) % mod;
    }
}

static inline uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    uint64_t f, f_next;
    fib_pair_uint64(n, mod, &f, &f_next);
    return f;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 2;; ++k) {                /* start from k = 2 (k-1 = 1) */
        uint64_t mod = k - 1;
        if (fib_mod_uint64(k, mod) == 0) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
