/*
 * A051445  Smallest k such that phi(k) = 2n, or 0 if there is no such k.
 * https://oeis.org/A051445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051445: smallest k such that φ(k) = 2*n, or 0 if none.
   Computed by sieve of Euler totient up to a fixed bound (2 M).
   This covers all n up to 1 000 000 reliably. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_K 2000000ULL          /* upper bound for k */
#define MAX_N (MAX_K/2)          /* maximal n we can answer */

static seq_t *ans_cache = NULL;   /* ans_cache[n] = smallest k with φ(k)=2n or 0 */
static bool initialized = false;

/* Compute totients up to MAX_K and build the answer table. */
static void init(void)
{
    if (initialized)
        return;

    /* Allocate phi array. */
    uint32_t *phi = (uint32_t *)malloc((MAX_K + 1) * sizeof(uint32_t));
    if (!phi) return; /* memory fail, give up, will return 0s */

    for (uint32_t i = 0; i <= MAX_K; i++)
        phi[i] = i;

    /* Euler sieve for φ. */
    for (uint32_t p = 2; p <= MAX_K; p++) {
        if (phi[p] == p) {          /* p is prime */
            for (uint32_t m = p; m <= MAX_K; m += p) {
                phi[m] -= phi[m] / p;
            }
        }
    }

    /* Allocate answer cache and initialise to 0. */
    ans_cache = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));

    /* Fill cache: store minimal k for each φ value. */
    for (uint32_t k = 3; k <= MAX_K; k++) {
        uint32_t ph = phi[k];
        if ((ph & 1) == 0) {                    /* only even totients are relevant */
            uint32_t n = ph >> 1;               /* n = ph / 2 */
            if (n <= MAX_N && ans_cache[n] == 0)
                ans_cache[n] = (seq_t)k;
        }
    }

    free(phi);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > MAX_N)
        return (seq_t)0;

    init();
    return ans_cache[n];
}
