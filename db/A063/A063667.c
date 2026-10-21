/*
 * A063667  Number of solutions of phi(x) = 12n + 2.
 * https://oeis.org/A063667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9532, u16 n <= 9532, u32 n <= 9532, u64 n <= 9532,
 *            u128 n <= 9532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = # of integers x such that φ(x) = 12*n + 2 */
#include <stdint.h>
#include <stdlib.h>

static int initialized = 0;
static uint32_t *phi = NULL;   /* Euler totient values */
static uint32_t *cnt  = NULL;  /* counts of each totient value up to MAXP */
static uint32_t MAXP = 0;      /* maximum φ value seen */

/* precomputes φ up to LIMIT and builds cnt */
static void init_totients(void)
{
    /* Limit should comfortably cover φ(x) = 12*n+2 for n up to about 1e5.
       5,000,000 is enough for that and keeps memory usage modest. */
    const size_t LIMIT = 5000000;
    phi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!phi) exit(1);
    for (size_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (phi[i] == i) {  /* i is prime */
            for (size_t j = i; j <= LIMIT; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    /* maximum φ encountered */
    MAXP = (uint32_t)LIMIT - 1;

    cnt = (uint32_t *)calloc(MAXP + 1, sizeof(uint32_t));
    if (!cnt) exit(1);
    for (size_t i = 1; i <= LIMIT; ++i) {
        uint32_t t = phi[i];
        if (t <= MAXP) cnt[t]++;   /* count each occurrence */
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_totients();

    /* φ value to look up */
    uint64_t m = (uint64_t)n * 12 + 2;   /* use 64‑bit for safety */
    if (m > MAXP) return 0;
    return (seq_t)cnt[(uint32_t)m];
}
