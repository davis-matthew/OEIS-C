/*
 * A106987  Primes of the form 5x^2-3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 5x^2 – 3xy + 5y^2, with x,y ≥
 *            0. The list is sorted in increasing order, offset 1
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------- static state ------------------------------ */
static bool _init = false;
static seq_t *primes  = NULL;   /* sorted list of primes found */
static size_t  primes_cnt  = 0;
static size_t  primes_cap  = 0;
static uint8_t *is_prime = NULL;  /* sieve array, 1 means prime */
static uint8_t *seen     = NULL;  /* marks values already used */

/* ----------------------------- utilities ------------------------------ */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return  1;
    return 0;
}

static void grow_primes(void)
{
    if (primes_cnt + 1 > primes_cap) {
        size_t newcap = primes_cap == 0 ? 1024 : primes_cap << 1;
        seq_t *newarr = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
        if (!newarr) abort();          /* out of memory */
        primes = newarr;
        primes_cap = newcap;
    }
}

/* --------------------------- initialization --------------------------- */
static void init(void)
{
    /* A generous upper bound that guarantees more than 10 000 terms. */
    const idx_t LIMIT = 20000000;   /* 20 million */

    /* sieve of Eratosthenes ----------------------------------------- */
    is_prime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    seen     = (uint8_t *)calloc((LIMIT + 1),  sizeof(uint8_t));
    if (!is_prime || !seen) abort();

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    if (LIMIT >= 0) is_prime[0] = 0;
    if (LIMIT >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    /* enumerate values of 5x^2 – 3xy + 5y^2 -------------------------- */
    idx_t max_x = 0;
    while ((max_x + 1) * (max_x + 1) * 5 <= LIMIT) max_x++;

    for (idx_t x = 0; x <= max_x; ++x) {
        for (idx_t y = 0; ; ++y) {
            seq_t val = (seq_t)5 * x * x - (seq_t)3 * x * y + (seq_t)5 * y * y;
            if (val == 0) continue;                     /* skip 0 */
            if ((idx_t)val > LIMIT) break;              /* no larger y works */
            idx_t u = (idx_t)val;
            if (is_prime[u] && !seen[u]) {
                seen[u] = 1;
                grow_primes();
                primes[primes_cnt++] = val;
            }
        }
    }

    /* sort the prime list -------------------------------------------- */
    qsort(primes, primes_cnt, sizeof(seq_t), cmp_seq);

    _init = true;
}

/* -------------------------- public function -------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1; outside sequence */
    if (!_init) init();
    if (n > primes_cnt) return 0;    /* if the chosen LIMIT is insufficient */
    return primes[n - 1];
}
