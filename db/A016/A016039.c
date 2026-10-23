/*
 * A016039  Inverse of 2030th cyclotomic polynomial.
 * https://oeis.org/A016039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in 1/Φ_2030(x), where Φ_2030 is the
 *            2030th cyclotomic polynomial. The sequence is periodic with
 *            period 2030. This implementation builds the period once and
 *            returns a[n % 2030] for any n
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define PERIOD 2030

/* Compute Möbius μ(n) for n <= 2030 */
static int mu_of(uint64_t n)
{
    int cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            ++cnt;
        }
    }
    if (n > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

/* Build the period array once */
static void build_period(seq_t period[PERIOD])
{
    for (int i = 0; i < PERIOD; ++i) period[i] = 0;
    period[0] = 1;

    const uint64_t M = 2030;
    /* Enumerate divisors of 2030 */
    for (uint64_t d = 1; d * d <= M; ++d) {
        if (M % d != 0) continue;

        uint64_t ds[2] = { d, M / d };
        for (int k = 0; k < (d == M / d ? 1 : 2); ++k) {
            uint64_t dd = ds[k];
            int mu = mu_of(M / dd);
            if (mu == 1) {
                /* Multiply by (1 - x^dd)^(-1) = 1 + x^dd + x^(2dd) + ... */
                for (int i = dd; i < PERIOD; ++i)
                    period[i] += period[i - dd];
            } else if (mu == -1) {
                /* Multiply by (1 - x^dd) = 1 - x^dd */
                for (int i = PERIOD - 1; i >= (int)dd; --i)
                    period[i] -= period[i - dd];
            }
            /* μ == 0 : ignore */
        }
    }
}

static seq_t period[PERIOD];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        build_period(period);
        initialized = true;
    }
    return period[n % PERIOD];
}
