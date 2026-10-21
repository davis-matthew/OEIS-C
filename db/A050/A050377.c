/*
 * A050377  Number of ways to factor n into "Fermi-Dirac primes" (members of
 *          A050376).
 * https://oeis.org/A050377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum{ a(d) : d^2 divides n } which makes a(n) a
 *            multiplicative function with a(p^e) = f[e] where f[0] = 1 and
 *            f[e] = Σ_{k=0}^{⌊e/2⌋} f[k] for all e ≥ 1
 * Range:     u8 n <= 9381, u16 n <= 9381, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t f[65];                /* pre‑computed f[e] values            */
static bool f_inited = false;      /* ensures the table is built once    */

static void init_f(void)
{
    if (f_inited) return;
    f_inited = true;
    f[0] = 1;
    for (idx_t e = 1; e < 65; ++e) {
        seq_t sum = 0;
        idx_t limit = e >> 1;      /* floor(e/2) */
        for (idx_t k = 0; k <= limit; ++k)
            sum += f[k];
        f[e] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* sequence is defined only for n≥1   */
    init_f();

    seq_t result = 1;
    idx_t tmp = n;

    /* factor out powers of 2 */
    if (tmp & 1) {   /* tmp is odd, skip 2 */
    } else {
        idx_t exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++exp;
        }
        result *= f[exp];
    }

    /* trial division by odd primes */
    for (idx_t p = 3; (p * p) <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= f[exp];
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {          /* exponent is 1, f[1] == 1, so no change */
    }

    return result;
}
