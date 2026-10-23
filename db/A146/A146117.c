/*
 * A146117  Bell numbers (A000110) read mod 27.
 * https://oeis.org/A146117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = B_n (Bell number) modulo 27. Periodicity: a(n+117)=a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* ----  Precomputation of Bell numbers modulo 27 up to period 117  ---- */
static bool init_done = false;
static seq_t bell[117];
static seq_t binom[117][117];   /* binom[n][k] = C(n,k) mod 27 */

/* Initialise binomial coefficients and Bell numbers once. */
static void init_sequence(void)
{
    if (init_done) return;

    /* Binomial coefficients modulo 27 using Pascal’s triangle. */
    for (idx_t n = 0; n <= 116; ++n) {
        binom[n][0] = 1;
        binom[n][n] = 1;
        for (idx_t k = 1; k < n; ++k) {
            binom[n][k] = (binom[n-1][k-1] + binom[n-1][k]) % 27;
        }
    }

    /* Bell numbers modulo 27 via Dobinski’s recurrence:
       B_{n+1} = sum_{k=0}^{n} C(n,k) * B_k (mod 27). */
    bell[0] = 1;
    for (idx_t n = 0; n < 116; ++n) {
        unsigned int sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            unsigned int prod = ((unsigned int)binom[n][k] *
                                 (unsigned int)bell[k]) % 27;
            sum += prod;
            sum %= 27;
        }
        bell[n + 1] = (seq_t)sum;
    }

    init_done = true;
}

/* ----  Public function required by the test harness  ---- */
seq_t a(idx_t n)
{
    init_sequence();
    idx_t idx = n % 117;
    return bell[idx];
}
