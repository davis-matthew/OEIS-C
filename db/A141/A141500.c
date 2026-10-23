/*
 * A141500  A triangular sequence of coefficients based on a skip prime prime
 *          power sequence: t(n,m)=Prime[m + 1]^n*Prime[m + 3]; qualified so
 *          the m=0 and n=0 terms are well-defined.
 * https://oeis.org/A141500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A141500
 *  a(n) = 1                      for n = 1
 *  a(n) = prime[n-1]             for n = 2,3 (row 1)
 *  a(n) = prime[n-3]             for n = 4–6 (row 2)
 *  a(n) = prime[c+1]^row * prime[c+3]  (row ≥3)
 *  where row = ⌊ (√(8n+1)−1)/2 ⌋ ,  c = n – (row*(row+1)/2) – 1
 *
 *  The function below computes the terms on demand, using an internal
 *  sieve that generates the first few hundred primes only once.
 *  It works for any seq_t/idx_t types supported by the build environment.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Function prototypes from the utilities provided by the build. */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp)      */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)          */
#include "../utils/seq_one.h"   /* seq_one(void)                     */

/* ---------- Prime number generation ---------- */

#define PRIME_SIEVE_LIMIT 2000   /* enough for all rows up to n≈10 000 */
static bool primes_ready = false;
static uint32_t primes[500];       /* 500 > number of primes ≤ 2000 */
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_ready) return;

    bool is_prime[PRIME_SIEVE_LIMIT + 1];
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;

    for (int p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (int q = p * p; q <= PRIME_SIEVE_LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    primes_cnt = 0;
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    primes_ready = true;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();           /* undefined for offset 0, safe */

    /* Find the row containing index n   (rows start at 0, length = row+1).  */
    idx_t r = 0;            /* provisional row (will be row+1 after loop) */
    while (r * (r + 1) / 2 < n) ++r;        /* r is smallest with T(r) ≥ n */
    idx_t row = r - 1;                         /* actual 0‑based row */

    /* Elements before this row */
    idx_t left = row * (row + 1) / 2;
    idx_t c    = n - left - 1;                 /* column in this row, 0‑based */

    /* Ensure we have enough primes for the required indices. */
    init_primes();                     /* primes[0] = 2, primes[1] = 3, … */

    if (row == 0) {
        return seq_one();                          /* a(1) = 1 */
    } else if (row == 1) {
        /* row 1: primes[0], primes[1] (first two primes) */
        return (seq_t)primes[c];
    } else if (row == 2) {
        /* row 2: primes[2], primes[3], primes[4] (indices 3,4,5) */
        return (seq_t)primes[c + 2];
    } else {
        /* general case: prime[c+1]^row * prime[c+3] */
        seq_t base = (seq_t)primes[c];         /* prime[c+1] */
        seq_t mul  = (seq_t)primes[c + 2];     /* prime[c+3] */

        seq_t powv = big_pow(base, (seq_t)row); /* base^row */
        return seq_mul(powv, mul);
    }
}
