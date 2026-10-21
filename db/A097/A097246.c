/*
 * A097246  Replace factors of n that are squares of a prime with the prime
 *          succeeding this prime.
 * https://oeis.org/A097246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e ‖ n} NextPrime(p)^{⌊e/2⌋} · p^{e mod 2}
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n = ∏ p_i^{e_i}, each pair p_i^2 is replaced by the next prime after p_i,
   and any remaining single factor p_i is kept.                          */

#include <stdbool.h>
#include <stdint.h>

/*---------- helpers ----------------------------------------------------*/

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n == 2 || n == 3)      return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return false;
    }
    return true;
}

static inline uint64_t next_prime_u64(uint64_t p)
{
    uint64_t cand = p + 1;
    while (!is_prime_u64(cand)) ++cand;
    return cand;
}

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= base;
    return r;
}

/*---------- sequence ---------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    uint64_t m   = (uint64_t)n;    /* factor while m > 1 */
    seq_t   res  = 1;

    /* handle factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            ++e;
            m /= 2;
        }
        if (e & 1) res *= (seq_t)2;               /* keep one 2 if e odd */
        if (e >= 2) {
            uint64_t np = next_prime_u64(2);
            int pairs   = e >> 1;                  /* floor(e/2) */
            res *= pow_seq_t((seq_t)np, (idx_t)pairs);
        }
    }

    /* odd factors */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d != 0) continue;
        int e = 0;
        while (m % d == 0) {
            ++e;
            m /= d;
        }
        if (e & 1) res *= (seq_t)d;               /* keep one d if e odd */
        if (e >= 2) {
            uint64_t np = next_prime_u64(d);
            int pairs   = e >> 1;                  /* floor(e/2) */
            res *= pow_seq_t((seq_t)np, (idx_t)pairs);
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* its exponent is 1; no pairs to replace */
        res *= (seq_t)m;
    }

    return res;
}
