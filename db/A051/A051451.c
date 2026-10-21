/*
 * A051451  a(n) = lcm{ 1,2,...,x } where x is the n-th prime power (A000961).
 * https://oeis.org/A051451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = lcm{1,2,…,x} where x is the n‑th prime power (A000961)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void init(void);
static int cmp_idx_t(const void *a, const void *b);

static bool ready = false;
static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t *prime_powers = NULL;
static size_t pp_cnt = 0;

static void init(void)
{
    if (ready) return;

    /* A safe upper bound for the 10 000‑th prime power */
    const idx_t LIMIT = 200000;

    /* Sieve of Eratosthenes: obtain all primes up to LIMIT */
    bool *comp = (bool*)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!comp) return;  /* allocation failure, give up */

    primes = (idx_t*)malloc(1024 * sizeof(idx_t));
    if (!primes) { free(comp); return; }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (prime_cnt >= 1024)
                primes = (idx_t*)realloc(primes, (prime_cnt + 1024) * sizeof(idx_t));
            primes[prime_cnt++] = i;

            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);

    /* Count prime powers up to LIMIT (including 1) */
    size_t count = 1;   /* for the power 1 */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t val = p;
        while (val <= LIMIT) {
            ++count;
            if (val > LIMIT / p) break;
            val *= p;
        }
    }

    prime_powers = (idx_t*)malloc(count * sizeof(idx_t));
    if (!prime_powers) { free(primes); return; }

    pp_cnt = 1;
    prime_powers[0] = 1;  /* include 1 as the first prime power */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t val = p;
        while (val <= LIMIT) {
            prime_powers[pp_cnt++] = val;
            if (val > LIMIT / p) break;
            val *= p;
        }
    }

    qsort(prime_powers, pp_cnt, sizeof(idx_t), cmp_idx_t);
    ready = true;
}

static int cmp_idx_t(const void *a, const void *b)
{
    idx_t aa = *(const idx_t *)a;
    idx_t bb = *(const idx_t *)b;
    return (aa < bb) ? -1 : (aa > bb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below the offset is irrelevant */

    init();
    if (!ready || n > pp_cnt) return 0;   /* should not happen for tested ranges */

    idx_t x = prime_powers[n - 1];

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt && primes[i] <= x; ++i) {
        idx_t p = primes[i];
        idx_t power = p;
        while (power * p <= x) power *= p;
        result *= (seq_t)power;
    }
    return result;
}
