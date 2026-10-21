/*
 * A099303  Greatest integer x such that x' = n, or 0 if there is no such x,
 *          where x' is the arithmetic derivative of x.
 * https://oeis.org/A099303
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A099303: for each n (n ≥ 2) the greatest integer x such that
   the arithmetic derivative of x equals n, or 0 if no such x exists. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Upper bound for n that will be queried (the offset is 2). */
#define NMAX 100000          /* safe for n up to about 10 000 */

/* Upper bound for x that we search.  Larger values are harmless but
   increase memory and time.  (n/2)^2 is an upper bound for even n,
   (n/3)^(3/2) for odd n; 25 million is comfortably above those for
   n ≤ NMAX. */
#define XMAX 25000000

/* Global tables used at most once.  They are created on first call. */
static bool          initialized = false;
static seq_t       *best   = NULL;    /* best[n] = largest x with x' = n   */
static uint32_t    *spf    = NULL;    /* smallest prime factor for 0..XMAX */

/* Build a linear sieve of smallest prime factors up to XMAX. */
static void build_spf(void)
{
    spf = (uint32_t *)malloc((XMAX + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= XMAX; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= XMAX; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= XMAX; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
}

/* Pre‑compute all values of the arithmetic derivative for x ≤ XMAX
   and store the greatest x producing each n in the table best[.]. */
static void precompute(void)
{
    if (initialized) return;

    build_spf();
    best = (seq_t *)calloc(NMAX + 1, sizeof(seq_t));

    for (size_t x = 2; x <= XMAX; ++x) {
        size_t temp = x;
        idx_t derivative = 0;          /* x' will always fit in idx_t */
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            derivative += (idx_t)cnt * (idx_t)(x / p);
        }
        if (derivative >= 2 && derivative <= (idx_t)NMAX) {
            size_t d = (size_t)derivative;  /* safe conversion to index */
            if (best[d] < (seq_t)x)
                best[d] = (seq_t)x;
        }
    }

    initialized = true;
}

/* Required external function. */
seq_t a(idx_t n)
{
    precompute();
    if (n < 2 || n > (idx_t)NMAX)
        return 0;
    return best[n];
}
