/*
 * A081371  Binomial coefficients C(n,j) reduced modulo j, j=1,...n; read by
 *          rows, j=0 is omitted because of mod(n,0).
 * https://oeis.org/A081371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r, j) mod j * where r is the row number such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2 * and j = n – r(r‑1)/2 (1‑based column)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Simple prime sieve up to PRIME_LIMIT (sufficient for n ≤ 10 000). */
#define PRIME_LIMIT 65536
#define PRIME_ARRAY_SIZE 7000
static int primes[PRIME_ARRAY_SIZE];
static size_t prime_cnt = 0;
static int sieve_done = 0;

static void init_primes(void)
{
    if (sieve_done) return;
    static unsigned char marked[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) marked[i] = 0;
    for (int p = 2; (long long)p * p <= PRIME_LIMIT; ++p) {
        if (!marked[p]) {
            for (long long k = (long long)p * p; k <= PRIME_LIMIT; k += p)
                marked[(int)k] = 1;
        }
    }
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT && idx < PRIME_ARRAY_SIZE; ++i) {
        if (!marked[i]) primes[idx++] = i;
    }
    prime_cnt = idx;
    sieve_done = 1;
}

static uint64_t modpow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset error guard */
    /* Find row r such that r(r‑1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;
    idx_t prev = (r - 1) * r / 2;           /* triangular number T(r‑1) */
    idx_t j    = n - prev;                  /* column position, 1‑based */
    if (j == 1) return (seq_t)0;            /* since x mod 1 == 0 */

    init_primes();                          /* ensures primes[ ] is ready */

    uint64_t mod   = (uint64_t)j;
    uint64_t result = 1 % mod;

    for (size_t pi = 0; pi < prime_cnt && primes[pi] <= r; ++pi) {
        int p = primes[pi];
        /* exponent of p in C(r,j) = v_p(r!) – v_p(j!) – v_p((r-j)!) */
        uint64_t e = 0, t;
        t = r;
        while (t) { t /= p; e += t; }
        t = j;
        while (t) { t /= p; e -= t; }
        t = r - j;
        while (t) { t /= p; e -= t; }
        if (e == 0) continue;              /* skip when exponent is zero */

        uint64_t pow = modpow_u64((uint64_t)p, e, mod);
        result = (uint64_t)((__uint128_t)result * pow % mod);
    }
    return (seq_t)result;
}
