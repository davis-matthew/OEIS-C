/*
 * A085496  Number of ways to write prime(n) as sum of distinct divisors of
 *          prime(n)+1.
 * https://oeis.org/A085496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write the n‑th prime as a sum of
 *            distinct divisors of that prime plus one
 * Range:     u8 n <= 40, u16 n <= 127, u32 n <= 472, u64 n <= 3597,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----------  Local prime generation (used to fetch nth prime)  ---------- */
static uint64_t *primes = NULL;    /* array of primes [0 .. prime_cnt-1]      */
static size_t   prime_cnt = 0;     /* number of primes stored                 */
static const size_t PRIME_LIMIT = 200000; /* sufficient for the first >20000 primes */

/* Build a simple sieve up to PRIME_LIMIT.  Called on first use. */
static void init_primes(void)
{
    if (primes != NULL) return;           /* already initialized */
    size_t limit = PRIME_LIMIT;
    char *is_comp = (char*)calloc(limit + 1, 1);
    if (!is_comp) exit(1);
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            ++cnt;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) primes[idx++] = (uint64_t)i;
    }
    prime_cnt = cnt;
    free(is_comp);
}

/* ----------  Recursive divisor generator  ---------- */
static void gen_divs(size_t idx, uint64_t cur,
                     const uint64_t *pr, const uint64_t *ex,
                     size_t cnt, uint64_t *out, size_t *pos)
{
    if (idx == cnt) {
        out[(*pos)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        gen_divs(idx + 1, cur * val, pr, ex, cnt, out, pos);
        val *= p;
    }
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* offset is 1 */
    init_primes();               /* ensure primes are ready */

    if ((size_t)n > prime_cnt) return 0;   /* safety, should not happen */

    uint64_t p = primes[(size_t)n - 1];    /* the n‑th prime   */
    uint64_t m = p + 1;                     /* prime + 1        */

    /* Factorise m */
    uint64_t pr[64], ex[64];
    size_t fac_cnt = 0;
    uint64_t tmp = m;
    for (size_t i = 0; i < prime_cnt && primes[i]*primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            uint64_t e = 0;
            do { tmp /= primes[i]; ++e; } while (tmp % primes[i] == 0);
            pr[fac_cnt]   = primes[i];
            ex[fac_cnt++] = e;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        pr[fac_cnt]   = tmp;
        ex[fac_cnt++] = 1;
    }

    /* Number of all divisors (including m itself) */
    size_t dcnt = 1;
    for (size_t i = 0; i < fac_cnt; ++i) dcnt *= (size_t)(ex[i] + 1);

    /* Generate all divisors */
    uint64_t *divs = (uint64_t*)malloc(dcnt * sizeof(uint64_t));
    if (!divs) return 0;                 /* allocation failure */
    size_t pos = 0;
    gen_divs(0, 1ULL, pr, ex, fac_cnt, divs, &pos);

    /* Dynamic programming over sums */
    size_t dp_size = (size_t)(p + 1);
    seq_t *dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(divs); return 0; }
    dp[0] = 1;                            /* empty sum */

    for (size_t i = 0; i < pos; ++i) {
        uint64_t d = divs[i];
        if (d == m) continue;            /* exclude divisor equal to p+1 */
        idx_t di = (idx_t)d;
        for (idx_t s = p; s >= di; --s) {
            dp[s] = dp[s] + dp[s - di];
        }
    }

    seq_t result = dp[p];

    free(dp);
    free(divs);

    return result;
}
