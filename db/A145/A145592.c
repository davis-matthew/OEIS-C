/*
 * A145592  a(n)=number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^14.
 * https://oeis.org/A145592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = count of k with 2 ≤ k ≤ N/p_n that are coprime * to all
 *            primes < p_n, where p_n is the nth prime and * N = 2^14. This
 *            equals the number of integers removed * in the nth step of
 *            Eratosthenes' sieve for 2^14
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define N 16384                      /* 2^14 */

/* Maximum number of primes <= N (≈1807). A generous upper bound. */
#define MAX_PRIMES 2000

static seq_t  cached_a[MAX_PRIMES];  /* memoised sequence values          */
static idx_t  prime_list[MAX_PRIMES];/* list of primes up to N             */
static idx_t  prime_cnt = 0;         /* number of primes found             */
static bool   initialised = false;   /* one‑time initialisation flag       */

/* One‑time initialisation: sieve of Eratosthenes and sequence computation. */
static void init_a145592(void)
{
    if (initialised) return;
    initialised = true;

    /* Sieve: comp[i]==true  <=>  i is composite. */
    static bool comp[N + 1];
    for (idx_t i = 0; i <= N; ++i) comp[i] = false;

    for (idx_t i = 2; i <= N; ++i) {
        if (!comp[i]) {                     /* i is prime                */
            if (prime_cnt < MAX_PRIMES)
                prime_list[prime_cnt++] = i; /* store it                   */

            if (i * i <= N) {               /* mark its multiples        */
                for (idx_t j = i * i; j <= N; j += i)
                    comp[j] = true;
            }
        }
    }

    /* Compute a(n) for each prime in order. */
    for (idx_t pi = 0; pi < prime_cnt; ++pi) {
        const idx_t p = prime_list[pi];
        const idx_t limit_k = N / p;                 /* max k for this prime   */
        seq_t cnt = 0;                               /* result for this prime  */

        /* Count k from 2..limit_k that are not divisible by any smaller prime. */
        for (idx_t k = 2; k <= limit_k; ++k) {
            bool ok = true;
            for (idx_t j = 0; j < pi; ++j) {
                if (k % prime_list[j] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) ++cnt;
        }
        cached_a[pi] = cnt;
    }
}

/* The external function required by the build. */
seq_t a(idx_t n)
{
    /* n < 1 never occurs in the OEIS data; return 0 for safety. */
    if (n < 1) return (seq_t)0;

    init_a145592();

    /* n > number of primes -> undefined for this N; return 0. */
    if (n > prime_cnt) return (seq_t)0;

    /* a(1) corresponds to prime_list[0], hence cached_a[n-1]. */
    return cached_a[n - 1];
}
