/*
 * A098508  Second column of the inverse of a Catalan scaled binomial matrix
 *          (A098505).
 * https://oeis.org/A098508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = numerator((n+1)*binomial(n,2)/binomial(2*n,n))
 * Range:     u8 n <= 25, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime infrastructure -------------------------------------- */

#define MAX_PRIME 20000            /* > n+1 for all n < 10000 */

static int  primes[MAX_PRIME];
static int  prime_index[MAX_PRIME + 1];
static int  prime_cnt = 0;

/* Build the prime table once.  The helper boolean array is a
 * compile-time constant, so the code is compatible with C11 and C++17. */
static void init_primes(void)
{
    if (prime_cnt > 0) return;            /* already initialized */

    static bool used[MAX_PRIME + 1] = {0};

    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!used[i]) {
            primes[prime_cnt] = i;
            prime_index[i]  = prime_cnt;  /* map prime to its index */
            ++prime_cnt;
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    used[j] = true;
            }
        }
    }
}

/* ---------- Legendre exponent of p in n! -------------------------------- */

static unsigned int vp_factorial(idx_t n, int p)
{
    unsigned int v = 0;
    idx_t div = p;
    while (div <= n) {
        v += n / div;
        div *= p;                /* div stays <= n because of the loop guard */
    }
    return v;
}

static unsigned int vp_binom(idx_t n, int p)
{
    /* exponent of p in binom(2n, n) = v_p((2n)!)-2*v_p(n!) */
    return vp_factorial(2 * n, p) - 2 * vp_factorial(n, p);
}

/* ---------- Main sequence function ------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(0)=0, a(1)=0 */

    init_primes();

    /* exponent array for the numerator product (n-1)*n*(n+1) */
    int expN[prime_cnt];
    for (int i = 0; i < prime_cnt; ++i) expN[i] = 0;

    /* factor each of the three consecutive numbers */
    for (int step = -1; step <= 1; ++step) {
        uint64_t x = (uint64_t)(n + step);   /* step = -1,0,1 */
        for (int j = 0; j < prime_cnt && (uint64_t)primes[j] * primes[j] <= x; ++j) {
            int p = primes[j];
            if (x % p == 0) {
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                expN[j] += cnt;
            }
        }
        if (x > 1) {                        /* remaining prime factor */
            int idx = prime_index[(int)x];
            expN[idx] += 1;                 /* exponent 1 */
        }
    }

    /* construct the reduced numerator */
    seq_t result = (seq_t)1;

    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        unsigned int eN = (unsigned int)expN[i];
        if (eN == 0) continue;             /* this prime does not occur */

        unsigned int eD = vp_binom(n, p);
        if (p == 2) ++eD;                  /* extra factor 2 in the denominator */

        unsigned int keep = eN;
        if (keep > eD) keep -= eD; else keep = 0;

        for (unsigned int k = 0; k < keep; ++k) {
            result *= (seq_t)p;
        }
    }

    return result;
}
