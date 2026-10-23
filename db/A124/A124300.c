/*
 * A124300  Greatest x such that mu(n+k) = mu(n) for 0<=k<=x, where
 *          mu=A008683.
 * https://oeis.org/A124300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = greatest x with μ(n+k)=μ(n) for 0≤k≤x
    (run length of equal Möbius values starting at n, minus 1) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static const size_t MAX = 20000;          /* enough for n ≤ 10000 */
static bool initialized = false;
static int *mu_int = NULL;               /* signed Möbius values, indices 1..MAX */
static seq_t *cache = NULL;              /* cached results, indices 1..MAX */

static void init(void)
{
    mu_int = (int *)malloc((MAX + 1) * sizeof(int));
    cache = (seq_t *)malloc((MAX + 1) * sizeof(seq_t));
    if (!mu_int || !cache) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= MAX; ++i) {
        mu_int[i] = 1;                     /* μ(1)=1, initial value for others */
        cache[i] = (seq_t)-1;              /* sentinel: not computed yet */
    }

    /* sieve for Möbius */
    bool *is_comp = (bool *)calloc(MAX + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    for (size_t p = 2; p <= MAX; ++p) {
        if (!is_comp[p]) {                 /* p is prime */
            for (size_t j = p; j <= MAX; j += p) {
                is_comp[j] = true;
                mu_int[j] *= -1;
            }
            unsigned long long p2 = (unsigned long long)p * p;
            if (p2 <= MAX) {
                for (size_t j = (size_t)p2; j <= MAX; j += (size_t)p2) {
                    mu_int[j] = 0;
                }
            }
        }
    }
    free(is_comp);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n < 1 || n > MAX) return (seq_t)0;   /* out of pre‑computed range */

    seq_t cached = cache[n];
    if (cached != (seq_t)-1) return cached;

    int mu_start = mu_int[n];
    size_t j = n;
    /* extend as long as the subsequent μ values are equal to μ_start */
    while (j + 1 <= MAX && mu_int[j + 1] == mu_start) ++j;

    seq_t val = (seq_t)(j - n);             /* run length minus 1 */
    cache[n] = val;
    return val;
}
