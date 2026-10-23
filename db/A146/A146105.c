/*
 * A146105  Bell numbers (A000110) read mod 15.
 * https://oeis.org/A146105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Bell numbers modulo 15.
 * Recurrence: B[0] = 1;
 * B[n+1] = Σ_{k=0}^{n} C(n,k) * B[k]  (mod 15).
 * The sequence is periodic with period 10153.
 * This implementation pre‑computes the entire period and caches it in
 * a static table.  All calculations are performed on 8‑bit values,
 * and the result is cast to seq_t when returned.
 */
#include <stdint.h>

#define PERIOD 10153U  /* length of the period */

/* cache of one full period of the sequence */
static uint8_t bell_cache[PERIOD];
/* current row of binomial coefficients (mod 15) */
static uint8_t binom[PERIOD];
/* temporary buffer to build the next binomial row */
static uint8_t tmp_binom[PERIOD];
/* flag indicating whether the cache has been initialized */
static uint8_t bell_initialized = 0;

/* initialise the sequence cache; called once by a() */
static void init_bell_cache(void)
{
    if (bell_initialized)
        return;

    bell_cache[0] = 1;   /* B[0] */
    binom[0] = 1;        /* C(0,0) */
    idx_t len = 1;       /* length of current binomial row */

    for (idx_t n = 0; n < PERIOD - 1; ++n) {
        /* compute B[n+1] = Σ_{k=0}^{n} C(n,k) * B[k] (mod 15) */
        uint8_t sum = 0;
        for (idx_t k = 0; k < len; ++k) {
            sum = (sum + binom[k] * bell_cache[k]) % 15;
        }
        bell_cache[n + 1] = sum;

        /* build next row of binomial coefficients modulo 15 */
        tmp_binom[0] = 1;
        for (idx_t k = 1; k < len; ++k) {
            tmp_binom[k] = (binom[k - 1] + binom[k]) % 15;
        }
        tmp_binom[len] = 1;

        /* copy the new row back into binom */
        for (idx_t k = 0; k <= len; ++k) {
            binom[k] = tmp_binom[k];
        }
        ++len;   /* row has increased by one */
    }

    bell_initialized = 1;
}

/* public function requested by the problem statement */
seq_t a(idx_t n)
{
    init_bell_cache();
    return (seq_t)bell_cache[n % PERIOD];
}
