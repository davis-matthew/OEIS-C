/*
 * A021584  Decimal expansion of 1/580.
 * https://oeis.org/A021584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) mod 5800) / 580 * Computes the n‑th digit
 *            (offset 0) of the decimal expansion of 1/580
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* constants */
    const unsigned long long MOD  = 5800ULL;
    const unsigned long long BASE = 10ULL;

    /* binary exponentiation to compute (10^(n+1)) % 5800 */
    unsigned long long res  = 1ULL;
    unsigned long long pow  = BASE;
    idx_t exp = n + 1;               /* n is idx_t, thus unsigned 64‑bit */

    while (exp > 0) {
        if (exp & 1ULL)
            res = (res * pow) % MOD;  /*(res,pow)<MOD so product fits in 64‑bit*/
        pow = (pow * pow) % MOD;
        exp >>= 1;
    }

    /* res == 10^(n+1) % 5800.  The desired digit is floor(res/580). */
    return (seq_t)(res / 580ULL);
}
