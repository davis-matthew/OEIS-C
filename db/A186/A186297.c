/*
 * A186297  a(n) = (A007520(n)-1)/2.
 * https://oeis.org/A186297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A007520(n)-1)/2
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A007520(n) here is the n-th prime congruent to 3 (mod 8). */

/* *************  prerequisites ************* */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* *************  static data structures ************* */
static idx_t *primes_all = NULL;   /* all primes found (for primality testing) */
static size_t primes_all_cnt = 0;
static size_t primes_all_cap = 0;

static idx_t *primes_3mod8 = NULL; /* primes == 3 (mod 8) */
static size_t primes_3mod8_cnt = 0;
static size_t primes_3mod8_cap = 0;

static idx_t last_checked = 3;      /* next odd candidate to test */
static bool primes_initialized = false;

/* *************  utility functions ************* */
static void primes_init(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    /* initialise array of all primes with the first prime 2 */
    primes_all_cap = 16;
    primes_all = (idx_t *)malloc(primes_all_cap * sizeof(idx_t));
    primes_all[0] = 2;
    primes_all_cnt = 1;

    /* initialise array for 3 (mod 8) primes (empty at start) */
    primes_3mod8_cap = 16;
    primes_3mod8 = (idx_t *)malloc(primes_3mod8_cap * sizeof(idx_t));
    primes_3mod8_cnt = 0;
}

/* Ensure we have at least n primes ≡ 3 (mod 8). */
static void ensure_primes_3mod8(idx_t n)
{
    if (!primes_initialized) primes_init();

    while (primes_3mod8_cnt < n)
    {
        idx_t cand = last_checked;          /* candidate prime */
        bool is_prime = true;

        /* trial division by all primes ≤ sqrt(cand) */
        for (size_t i = 0; i < primes_all_cnt; ++i)
        {
            idx_t p = primes_all[i];
            if (p > cand / p) break;           /* p*p > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* add to primes_all */
            if (primes_all_cnt == primes_all_cap) {
                primes_all_cap = primes_all_cap ? primes_all_cap << 1 : 16;
                primes_all = (idx_t *)realloc(primes_all,
                        primes_all_cap * sizeof(idx_t));
            }
            primes_all[primes_all_cnt++] = cand;

            /* add to primes_3mod8 if congruent to 3 mod 8 */
            if ((cand & 7ULL) == 3) {   /* cand % 8 == 3 */
                if (primes_3mod8_cnt == primes_3mod8_cap) {
                    primes_3mod8_cap = primes_3mod8_cap ? primes_3mod8_cap << 1 : 16;
                    primes_3mod8 = (idx_t *)realloc(primes_3mod8,
                            primes_3mod8_cap * sizeof(idx_t));
                }
                primes_3mod8[primes_3mod8_cnt++] = cand;
            }
        }

        last_checked += 2;         /* next odd number */
    }
}

/* *************  sequence function ************* */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    ensure_primes_3mod8(n);

    seq_t val = ((seq_t)primes_3mod8[n - 1] - (seq_t)1) / (seq_t)2;
    return val;
}
