/*
 * A165576  Partial sums of A165574.
 * https://oeis.org/A165576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (k | 263), the Legendre symbol modulo 263. The
 *            sequence of Legendre symbols has period 263 and its partial sums
 *            are periodic with the same period, since Σ_{k=1}^{263} (k|263) =
 *            0
 * Range:     u8 n <= 9807, u16 n <= 9807, u32 n <= 9807, u64 n <= 9807,
 *            u128 n <= 9807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define P 263                      /* period of the sequence */

/* static storage for the period */
static bool      resid[P]           = {false};
static seq_t    sums[P]             = {0};
static bool      initialized        = false;

/* one‑time constructor: compute residues and partial sums */
static void init_sequence(void)
{
    if (initialized)
        return;

    /* mark quadratic residues modulo P */
    for (idx_t k = 1; k < P; ++k) {
        idx_t r = (k * k) % P;
        resid[r] = true;
    }

    /* build the partial sums  S(0)=0 , S(i)=S(i‑1)+χ(i)  */
    int sum = 0;                            /* signed to allow -1 */
    sums[0] = 0;
    for (idx_t i = 1; i < P; ++i) {
        int term = resid[i] ? 1 : -1;
        sum += term;
        sums[i] = (seq_t)sum;
    }

    initialized = true;
}

/* public function */
seq_t a(idx_t n)
{
    init_sequence();                     /* guarantee the tables exist */
    idx_t r = n % P;                     /* periodicity */
    return sums[r];
}
