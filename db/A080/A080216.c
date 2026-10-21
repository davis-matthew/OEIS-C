/*
 * A080216  a(n) is the largest value taken by binomial(n,j) mod j for j in
 *          [1..n].
 * https://oeis.org/A080216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1 <= j <= n} binomial(n, j) mod j Computation uses
 *            prime‑factorisation of binomial coefficients. The routine is O(n
 *            * number_of_primes) per call and works for any seq_t type. It
 *            precomputes factorial exponents for all integers up to 10000
 * Range:     u8 n <= 284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*---------------------------------------------------------*/
/*  Prime sieve and factorial‑exponent precomputation     */
/*---------------------------------------------------------*/

#define MAX_N       10000          /* maximum n considered   */
#define MAX_PRIMES  1229           /* primes up to 10000     */

static int  primeCnt    = 0;
static int  primes[MAX_PRIMES];
static int  primeIdx[MAX_N + 1];   /* prime -> index in primes[]   */
static uint16_t factExp[MAX_PRIMES][MAX_N + 1]; /* factExp[pi][k] = exponent of primes[pi] in k! */

static void precompute(void)
{
    if (primeCnt) return;  /* already done */

    /* ---------- sieve for primes and spf ---------- */
    static int spf[MAX_N + 1];
    for (int i = 0; i <= MAX_N; ++i) spf[i] = i;

    for (int i = 2; i * i <= MAX_N; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    for (int i = 2; i <= MAX_N; ++i)
        if (spf[i] == i) {
            primes[primeCnt++] = i;
            primeIdx[i] = primeCnt - 1;
        }

    /* ---------- factorial exponents ---------- */
    for (int pi = 0; pi < primeCnt; ++pi) factExp[pi][0] = 0;

    uint16_t curExp[MAX_PRIMES] = {0};

    for (int k = 1; k <= MAX_N; ++k) {
        int m = k;
        while (m != 1) {
            int p = spf[m];
            int idx = primeIdx[p];
            int cnt = 0;
            while (m % p == 0) { ++cnt; m /= p; }
            curExp[idx] += cnt;
        }
        for (int pi = 0; pi < primeCnt; ++pi)
            factExp[pi][k] = curExp[pi];
    }
}

/*---------------------------------------------------------*/
/*   modular exponentiation (base <= MOD, MOD <= 10000)   */
/*---------------------------------------------------------*/
static uint64_t mod_pow_uint32(uint32_t base, int exp, uint64_t mod)
{
    if (mod <= 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/*---------------------------------------------------------*/
/*  Main sequence routine                                 */
/*---------------------------------------------------------*/
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) { precompute(); initialized = 1; }

    seq_t maxVal = 0;

    for (idx_t j = 1; j <= n; ++j) {
        uint64_t mod = (uint64_t)j;
        uint64_t res = (mod <= 1) ? 0 : 1 % mod;

        if (mod > 1) {
            for (int pi = 0; pi < primeCnt; ++pi) {
                int e = (int)factExp[pi][n] - (int)factExp[pi][j]
                                - (int)factExp[pi][n - j];
                if (e <= 0) continue;
                uint64_t term = mod_pow_uint32(primes[pi], e, mod);
                res = (res * term) % mod;
            }
        }

        if (res > maxVal) maxVal = res;
    }

    return (seq_t)maxVal;
}
