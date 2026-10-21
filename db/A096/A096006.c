/*
 * A096006  Triangle read by rows: T(n,k) = largest prime factor of
 *          binomial(n,k), 1 <= k <= n-1.
 * https://oeis.org/A096006
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 1276, u16 n <= 1276, u32 n <= 1276, u64 n <= 1276,
 *            u128 n <= 1276, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A096006
    Compute a(n) = largest prime factor of binomial(r,k) where
    (r,k) is obtained from index n by reading the triangle
    of largest prime factors of binomial coefficients row‑wise.
    The triangle is indexed starting at n=2 (first term).
    The implementation uses a small prime sieve and Legendre's
    formula to evaluate the largest prime factor without
    forming the binomial coefficient itself. */

#include <stddef.h>
#include <stdbool.h>

/*   seq_t and idx_t are supplied by the build.  */

static unsigned primes_buf[2000];
static size_t  primes_cnt = 0;
static bool    primes_done = false;

/*   Initialise a sieve of primes up to 2000.   */
static void init_primes(void)
{
    if (primes_done) return;
    const unsigned LIMIT = 2000;
    static bool small[2001] = {0};           /* zero‑initialised statically */

    for (unsigned i = 2; i <= LIMIT; ++i) {
        if (!small[i]) {
            primes_buf[primes_cnt++] = i;
            if ((unsigned long long)i * i <= LIMIT)
                for (unsigned j = i * i; j <= LIMIT; j += i)
                    small[j] = true;
        }
    }
    primes_done = true;
}

/*   Largest prime factor of binomial(r,k) via Legendre's exponent
    formula.  The function assumes r>=2 and 1<=k<=r-1.   */
static seq_t largest_prime_factor_binom(idx_t r, idx_t k)
{
    if (k > r - k) k = r - k;               /* use symmetry to minimise k */

    seq_t maxp = 0;
    for (size_t i = 0; i < primes_cnt && primes_buf[i] <= r; ++i) {
        unsigned p = primes_buf[i];
        idx_t e = 0, power = p;
        while (power <= r) {
            e += r / power;
            e -= k / power;
            e -= (r - k) / power;
            if (power > r / p) break;     /* stop before overflow */
            power *= p;
        }
        if (e > 0 && p > maxp) maxp = p;
    }
    return maxp;                            /* for r>=2, maxp >= 2 */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* values below the offset are irrelevant */
    init_primes();

    /*   Convert linear index (n-2) to row r and column k   */
    idx_t t = n - 2;                         /* 0‑based position in the triangle   */
    idx_t r = 2;
    while ((r - 1) * r / 2 <= t) ++r;        /* find minimal r with cumulative > t */

    idx_t prev = (r - 2) * (r - 1) / 2;      /* entries up to previous row      */
    idx_t pos  = t - prev;                   /* 0‑based position in row r      */
    idx_t k    = pos + 1;                    /* convert to 1‑based column       */

    return largest_prime_factor_binom(r, k);
}
