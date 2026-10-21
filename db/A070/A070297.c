/*
 * A070297  Smallest number with n unrelated numbers (numbers which are
 *          neither coprime nor a divisor), or 0 if no such number exists.
 * https://oeis.org/A070297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that m - φ(m) - d(m) + 1 = n (φ = Euler
 *            totient, d = number of positive divisors)
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* The type seq_t and idx_t are defined by the build system. */

static seq_t *min_for_n = NULL;   /* mapping: U(m) -> smallest m (0 = none) */
static size_t max_n = 200000;     /* maximal n we index (change if needed) */
static bool init_done = false;    /* one‑time initialisation flag */

/* One-time build of φ, d and the mapping up to LIMIT_M. */
static void init(void)
{
    /* Limits: adjust if a larger range of n is required. */
    const idx_t LIMIT_M = 1200000u;           /* ≤ 1.2 million is fast enough */
    const size_t MAP_SIZE = max_n + 1;

    /* Allocate mapping array (0 means “no m found yet”). */
    min_for_n = (seq_t *)calloc(MAP_SIZE, sizeof(seq_t));
    if (!min_for_n) exit(EXIT_FAILURE);

    /* Allocate temporary arrays for linear sieve. */
    uint32_t *phi = (uint32_t *)calloc(LIMIT_M + 1, sizeof(uint32_t));
    uint32_t *d   = (uint32_t *)calloc(LIMIT_M + 1, sizeof(uint32_t));
    uint32_t *spf = (uint32_t *)calloc(LIMIT_M + 1, sizeof(uint32_t));
    uint8_t  *exp = (uint8_t  *)calloc(LIMIT_M + 1, sizeof(uint8_t));
    if (!phi || !d || !spf || !exp) exit(EXIT_FAILURE);

    /* Small array of primes for linear sieve (over‑allocate). */
    uint32_t *primes = (uint32_t *)malloc((LIMIT_M / 10) * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t pcnt = 0;

    /* Initialise for m = 1. */
    phi[1] = 1;
    d[1]   = 1;
    spf[1] = 0;
    exp[1] = 0;

    /* Linear sieve to compute φ, d, and spf. */
    for (idx_t i = 2; i <= LIMIT_M; ++i) {
        if (!spf[i]) {                         /* i is prime */
            spf[i] = (uint32_t)i;
            exp[i] = 1;
            phi[i] = (uint32_t)i - 1;
            d[i]   = 2;
            primes[pcnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = primes[j];
            idx_t ip = i * (idx_t)p;
            if (ip > LIMIT_M) break;
            spf[ip] = p;
            if (i % p == 0) {                  /* p divides i */
                exp[ip] = exp[i] + 1;
                d[ip]   = d[i] / (exp[i] + 1) * (exp[ip] + 1);
                phi[ip] = phi[i] * (idx_t)p;
                break;
            } else {                           /* p does not divide i */
                exp[ip] = 1;
                d[ip]   = d[i] * 2;
                phi[ip] = phi[i] * (p - 1);
            }
        }
    }

    /* Build the mapping U(m) -> minimal m. */
    for (idx_t m = 3; m <= LIMIT_M; ++m) {     /* m ≥ 3 because a(0)=3 */
        idx_t U = m - phi[m] - d[m] + 1;
        if (U <= max_n && min_for_n[U] == 0) {
            min_for_n[U] = (seq_t)m;          /* store the minimal m */
        }
    }

    /* Free temporary arrays (min_for_n must stay). */
    free(phi); free(d); free(spf); free(exp); free(primes);

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init();
    if (n >= max_n + 1) return (seq_t)0;
    return min_for_n[n];
}
