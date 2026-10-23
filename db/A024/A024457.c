/*
 * A024457  [ (4th elementary symmetric function of P(n))/(3rd elementary
 *          symmetric function of P(n)) ], where P(n) = {first n+3 primes}.
 * https://oeis.org/A024457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (fourth elementary symmetric function of the first n+3
 *            primes) divided by (third elementary symmetric function of the
 *            first n+3 primes)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple sieve to pre‑compute a fixed number of primes.
   The sieve is performed only once (first call).  The count of primes is
   sufficient for n up to about 10 000.  For larger n the function still
   works but may exceed the pre‑computed limit. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* static storage for the prime table */
static uint32_t *prime_list = NULL;   /* array of generated primes          */
static size_t    prime_count = 0;     /* number of primes stored            */
static bool      primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;

    /* 150_000 is well above the 10 000th prime (104 729) + a safety margin */
    const int LIMIT = 150000;
    bool *mark = (bool*)malloc((size_t)LIMIT + 1);
    if (!mark)
        return;                          /* allocation failed – silent exit */

    memset(mark, 0, (size_t)LIMIT + 1);

    /* Sieve of Eratosthenes – count primes first */
    size_t cnt = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        if (!mark[i]) {
            ++cnt;
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    mark[j] = true;
            }
        }
    }

    prime_count = cnt;
    prime_list = (uint32_t*)malloc(sizeof(uint32_t) * prime_count);
    if (!prime_list) {
        free(mark);
        return;                          /* allocation failed – silent exit */
    }

    /* Store primes */
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        if (!mark[i])
            prime_list[idx++] = (uint32_t)i;
    }

    free(mark);
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1, return 0 */

    init_primes();                      /* ensure primes are ready */
    if (!primes_ready || prime_count < (size_t)(n + 3))
        return (seq_t)0;                 /* insufficient primes            */

    const size_t m = (size_t)(n + 3);    /* number of primes to use        */

    seq_t e1 = (seq_t)0;                /* elementary symmetric sums     */
    seq_t e2 = (seq_t)0;
    seq_t e3 = (seq_t)0;
    seq_t e4 = (seq_t)0;

    for (size_t i = 0; i < m; ++i) {
        seq_t q = (seq_t)prime_list[i];
        e4 = e4 + q * e3;
        e3 = e3 + q * e2;
        e2 = e2 + q * e1;
        e1 = e1 + q;
    }

    return e4 / e3;                     /* exact division in seq_t type   */
}
