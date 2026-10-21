/*
 * A094042  Beatty sequence for e^Pi - Pi^e - i^i.
 * https://oeis.org/A094042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * (1/(1-(e^π - π^e - i^i))) )
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Beatty sequence for e^π - π^e - i^i */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The Beatty constant β = 1/(1 - (e^π - π^e - i^i)) */
    static bool initialized = false;
    static long double beta = 0.0L;

    if (!initialized)
    {
        /* π as a long double */
        const long double pi = acosl(-1.0L);

        /* e^π */
        const long double e_pow_pi = expl(pi);

        /* π^e – use powl with the real exponent e */
        const long double pi_pow_e = powl(pi, expl(1.0L));

        /* i^i = e^{-π/2} (real part) */
        const long double i_pow_i = expl(-pi / 2.0L);

        /* α = e^π - π^e - i^i */
        const long double alpha = e_pow_pi - pi_pow_e - i_pow_i;

        /* β = 1 / (1 - α) */
        beta = 1.0L / (1.0L - alpha);

        initialized = true;
    }

    /* Compute floor(n * β) with a tiny epsilon to guard against
       round‑off that could push a value just below an integer. */
    const long double prod = (long double)n * beta + 1e-14L;
    const uint64_t val = (uint64_t)floorl(prod);

    return (seq_t)val;
}
