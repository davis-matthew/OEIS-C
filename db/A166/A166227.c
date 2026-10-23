/*
 * A166227  Let dsf(n) = n_1^{n_1}+n_2^{n_2}+n_3^{n_3} + n_m^{n_m}, where
 *          {n_1,n_2,n_3,...n_m} is the list of the digits of an integer n.
 *          dsf(793312220) = 388244100 and dsf(388244100) = 33554978, ...,
 *          dsf(387467199) = 793312220, ... in this way these 40 numbers make
 *          a loop for the function dsf.
 * https://oeis.org/A166227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = dsf applied iteratively; the orbit has period 40
 * Range:     u8 none, u16 none, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"   /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */

#define PERIOD 40

/* Compute dsf(x) = Σ d^d over the decimal digits of x. */
static seq_t dsf(seq_t x)
{
    uint8_t buf[64];
    size_t len = 0;
    get_digits(x, buf, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t d = (seq_t)buf[i];
        sum += big_pow(d, d);
    }
    return sum;
}

/* Sequence function a(n) for the dsf orbit. */
seq_t a(idx_t n)
{
    static seq_t orbit[PERIOD];
    static bool initialized = false;

    if (!initialized) {
        /* Initialize the orbit starting from the seed. */
        orbit[0] = (seq_t)793312220U;
        for (idx_t i = 1; i < PERIOD; ++i) {
            orbit[i] = dsf(orbit[i - 1]);
        }
        initialized = true;
    }

    /* Map n to a 0‑based index modulo the cycle length. */
    idx_t idx = (n - 1) % PERIOD;
    return orbit[idx];
}
