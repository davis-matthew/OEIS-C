/*
 * A146115  Bell numbers (A000110) read mod 25.
 * https://oeis.org/A146115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell(n) mod 25, periodic with period 3905
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define PERIOD 3905

/* Static tables for the first period of the sequence */
static seq_t bell[PERIOD];
static seq_t coeff[PERIOD];
static seq_t newcoeff[PERIOD];
static int initialized = 0;

/* Pre‑compute the period using the recurrence
   B(0) = 1
   B(n+1) = Σ_{k=0}^n C(n,k) * B(k)   (mod 25)   */
static void init_bell(void)
{
    if (initialized) return;
    initialized = 1;

    seq_t *prev = coeff;
    seq_t *curr = newcoeff;

    bell[0] = 1;
    prev[0] = 1;          /* row for n = 0 : C(0,0) = 1 */

    for (size_t n = 0; n < PERIOD - 1; ++n) {
        seq_t next = 0;
        for (size_t k = 0; k <= n; ++k) {
            unsigned int prod = (unsigned int)bell[k] * (unsigned int)prev[k];
            next = (next + prod) % 25;
        }
        bell[n + 1] = next;

        /* Build the next row of binomial coefficients modulo 25 */
        curr[0] = 1;
        curr[n + 1] = 1;
        for (size_t k = 1; k < n + 1; ++k) {
            curr[k] = (prev[k - 1] + prev[k]) % 25;
        }

        /* Swap the role of the rows for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
}

/* External entry point required by the framework */
seq_t a(idx_t n)
{
    init_bell();
    size_t idx = (size_t)(n % PERIOD);
    return bell[idx];
}
