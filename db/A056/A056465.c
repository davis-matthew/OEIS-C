/*
 * A056465  Number of primitive (aperiodic) palindromes using exactly four
 *          different symbols.
 * https://oeis.org/A056465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} μ(d) * A056455(n/d) (b(n) = A056455(n) – number
 *            of length‑n palindromes over 4 symbols that use all 4 symbols)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal includes for integer types */
#include <stdint.h>

/*----- Helper: power of an unsigned seq_t --------*/
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/*----- Helper: number of length‑m palindromes over 4 symbols
          that use all 4 symbols (A056455) --------*/
static seq_t count_b(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = (n + 1) / 2;           /* ceil(n/2) */
    seq_t p4 = pow_seq_t((seq_t)4, m);
    seq_t p3 = pow_seq_t((seq_t)3, m);
    seq_t p2 = pow_seq_t((seq_t)2, m);
    return p4 - (seq_t)4 * p3 + (seq_t)6 * p2 - (seq_t)4 * 1;
}

/*----- Helper: Möbius function μ(n) (trial division) --------*/
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int prime_factors = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1) return 0;   /* square factor → μ=0 */
            }
            ++prime_factors;
        }
    }
    if (n > 1) ++prime_factors;           /* remaining prime factor */
    return (prime_factors & 1) ? -1 : 1;   /* (-1)^k */
}

/*----- Main sequence function ------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1 */

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        int mu_d = mobius(d);
        if (mu_d) {
            idx_t nd = n / d;
            seq_t bd = count_b(nd);
            if (mu_d > 0) pos += bd;
            else          neg += bd;
        }

        idx_t o = n / d;
        if (o == d) continue;               /* already processed */

        int mu_o = mobius(o);
        if (mu_o) {
            idx_t nd = n / o;                /* equals d */
            seq_t bd = count_b(nd);
            if (mu_o > 0) pos += bd;
            else          neg += bd;
        }
    }

    /* pos should be >= neg, but guard against underflow */
    return (pos >= neg) ? (pos - neg) : 0;
}
