/*
 * A157077  Triangle read by rows, coefficients of the Legendre polynomials
 *          P(n, x) times 2^n: T(n, k) = 2^n * [x^k] P(n, x), n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A157077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 49, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A157077 – Triangle read by rows, coefficients of 2^n·P(n,x)  */
/*  Computes the flattened sequence a(n),  n>=0.                         */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- utilities from the build  ---------- */
#include "../utils/seq_pow.h"  /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"  /* seq_mul(seq_t a, seq_t b)    */

/* ---------- prime table up to 20000 ---------- */
#define MAX_PRIME 20000
#define PRIME_BUF 3000
static uint32_t primes[PRIME_BUF];
static size_t primes_cnt = 0;
static bool primes_ready = false;

/* compute primes up to MAX_PRIME (simple sieve) */
static void init_primes(void)
{
    if (primes_ready) return;
    bool is_comp[MAX_PRIME + 1] = {0};
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = true;
        }
    primes_ready = true;
}

/* exponent of prime p in m! */
static int fac_prime_exp(int m, uint32_t p)
{
    int exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

/* ---------- main OEIS function ---------- */
seq_t a(idx_t n)
{
    /* locate the row r and column k for flattened index n */
    idx_t lo = 0, hi = n;                  /* r never exceeds n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    idx_t k = n - r * (r + 1) / 2;

    /* parity check: r-k must be even */
    if (((r - k) & 1U) != 0)
        return (seq_t)0;

    idx_t j = (r - k) >> 1;                /* j = (r-k)/2 */

    /* prepare prime table */
    init_primes();

    /* compute result as product over primes */
    seq_t res = (seq_t)1;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        int e = fac_prime_exp((int)(r + k), p)
                - fac_prime_exp((int)j, p)
                - fac_prime_exp((int)((r + k) >> 1), p)
                - fac_prime_exp((int)k, p);
        if (e > 0) {
            seq_t p_pow = seq_pow((seq_t)p, (idx_t)e);
            res = seq_mul(res, p_pow);
        }
    }

    /* apply sign if needed */
    if ((j & 1U) != 0)
        res = -res;

    return res;
}
