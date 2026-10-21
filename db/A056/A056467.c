/*
 * A056467  Number of primitive (aperiodic) palindromes using exactly six
 *          different symbols.
 * https://oeis.org/A056467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 24, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{d|n} mu(d) * A056457(n/d)
 * where A056457(m) = Sum_{k=0..6} (-1)^k * C(6,k) * (6-k)^{ceil(m/2)}
 * This implementation precomputes the Möbius function up to a fixed limit
 * and evaluates the sum directly for each query.
 */

#include <stddef.h>
#include <stdbool.h>

/* Types are provided by the build system (seq_t, idx_t). */

/* ----------  Precomputation of primes and Möbius function ---------- */

#define MU_LIMIT 100000   /* sufficient for n up to ~10^4 */
static int mobius[MU_LIMIT + 1];
static int primes[9592];  /* number of primes <= 100000 */
static int prime_cnt = 0;
static bool mobius_ready = false;

static void init_mobius(void)
{
    if (mobius_ready) return;      /* already initialized */
    /* simple sieve for primes up to MU_LIMIT */
    bool is_composite[MU_LIMIT + 1] = {0};
    for (idx_t i = 2; i <= MU_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = (int)i;
            if (i * i <= MU_LIMIT) {
                for (idx_t j = i * i; j <= MU_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    /* compute Möbius function */
    mobius[1] = 1;
    for (int n = 2; n <= MU_LIMIT; ++n) {
        int tmp = n;
        int cnt = 0;
        bool squarefree = true;
        for (int pi = 0; pi < prime_cnt && (idx_t)primes[pi] * (idx_t)primes[pi] <= (idx_t)tmp; ++pi) {
            int p = primes[pi];
            if (tmp % p == 0) {
                ++cnt;
                tmp /= p;
                if (tmp % p == 0) {
                    squarefree = false;
                    break;
                }
            }
        }
        if (squarefree && tmp > 1) ++cnt;
        mobius[n] = squarefree ? ( (cnt & 1) ? -1 : 1 ) : 0;
    }
    mobius_ready = true;
}

/* ----------  Basic big-integer exponentiation (seq_t) ---------- */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* ----------  Count of palindromes using all 6 symbols ---------- */

static const int comb6[7] = {1, 6, 15, 20, 15, 6, 1};

static seq_t count_all_symbols(idx_t m)
{
    /* independent positions for palindrome: ceil(m/2) */
    idx_t half = (m + 1) / 2;
    seq_t sum = 0;
    for (int k = 0; k <= 6; ++k) {
        seq_t term = pow_seq((seq_t)(6 - k), half);
        term *= (seq_t)comb6[k];
        if (k & 1)
            sum -= term;      /* (-1)^k */
        else
            sum += term;
    }
    return sum;
}

/* ----------  Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for n < offset */
    init_mobius();                  /* ensure mobius array ready */
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            int mu = mobius[d];
            if (mu != 0) {
                idx_t m = n / d;
                seq_t val = count_all_symbols(m);
                total += (mu > 0) ? val : -val;
            }
            idx_t d2 = n / d;
            if (d2 != d) {
                mu = mobius[d2];
                if (mu != 0) {
                    idx_t m = n / d2;
                    seq_t val = count_all_symbols(m);
                    total += (mu > 0) ? val : -val;
                }
            }
        }
    }
    return total;
}
