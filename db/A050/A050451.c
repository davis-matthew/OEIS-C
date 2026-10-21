/*
 * A050451  a(n) = Sum_{d|n, d=1 mod 4} d^3.
 * https://oeis.org/A050451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d≡1 (mod 4)} d³
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – built once to supply small primes for factorisation   */
static int  *prime_list = NULL;
static size_t prime_cnt  = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;                       /* already initialised */

    /* Limit large enough for all n tested (n≈10⁴ → √n≈100, so 100 000 is safe) */
    const int SIEVE_LIMIT  = 100000;

    /* Allocate a boolean array; cast is needed for C++ */
    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, 1);
    if (is_comp == NULL)
        return;                       /* allocation failure – ignore */

    /* Simple sieve of Eratosthenes */
    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes for allocation */
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            ++prime_cnt;

    /* Allocate and fill the prime list */
    prime_list = (int *)malloc(prime_cnt * sizeof(int));
    if (prime_list == NULL) {
        free(is_comp);
        return;                       /* allocation failure – ignore */
    }

    size_t idx = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i])
            prime_list[idx++] = i;
    }
    free(is_comp);
}

/* --------------------------------------------------------------------- */
/* Recursive enumeration of all divisors, accumulating the cube sum    */
static void dfs_div(int pos, seq_t cur,
                    const unsigned long long *pf,
                    const int *pe,
                    int sz, seq_t *sum)
{
    if (pos == sz) {
        if ((cur & 3ULL) == 1ULL)          /* d ≡ 1 (mod 4) */
            *sum += cur * cur * cur;       /* all arithmetic in seq_t */
        return;
    }

    seq_t mult = 1;
    for (int e = 0; e <= pe[pos]; ++e) {
        dfs_div(pos + 1, cur * mult, pf, pe, sz, sum);
        mult *= (seq_t)pf[pos];
    }
}

/* --------------------------------------------------------------------- */
/* Compute a(n) by factorising n and summing the required cubes         */
static seq_t compute_a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n=0, but harmless */

    const int MAX_FACTORS = 32;
    unsigned long long pf[MAX_FACTORS];
    int            pe[MAX_FACTORS];
    int            sz = 0;
    unsigned long long m = (unsigned long long)n;

    init_primes();                   /* ensure primes are ready */

    for (size_t i = 0; i < prime_cnt && (unsigned long long)prime_list[i] *
         (unsigned long long)prime_list[i] <= m; ++i) {
        if (m % prime_list[i] == 0) {
            if (sz >= MAX_FACTORS)
                break;               /* unlikely for n ≤ 10 000 */
            pf[sz] = (unsigned long long)prime_list[i];
            int cnt = 0;
            while (m % prime_list[i] == 0) {
                m /= prime_list[i];
                ++cnt;
            }
            pe[sz++] = cnt;
        }
    }

    if (m > 1ULL) {                    /* remaining prime factor */
        if (sz < MAX_FACTORS) {
            pf[sz] = m;
            pe[sz] = 1;
            ++sz;
        }
    }

    seq_t sum = 0;
    dfs_div(0, (seq_t)1, pf, pe, sz, &sum);
    return sum;
}

/* --------------------------------------------------------------------- */
/* Public entry point – called by the testing harness                  */
seq_t a(idx_t n)
{
    return compute_a(n);
}
