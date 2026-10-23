/*
 * A192543  Let r be the largest real zero of x^n - x^(n-1) - x^(n-2) - ... -
 *          1 = 0. Then a(n) is the value of k which satisfies the equation
 *          0.5/10^k < 2 - r < 5/10^k.
 * https://oeis.org/A192543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 848, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A192543
    a(1) = 0
    a(n) = ceil((n-1)*log10(2))   for n ≥ 2
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Scale factor 10^18 (fits in 64‑bit).  The constant below
       is floor(log10(2) * 10^18).  It is about 0.301029995663981195…
    */
    const unsigned long long SCALE      = 1000000000000000000ULL;
    const unsigned long long LOG2_SCALED = 301029995663981195ULL;

    unsigned long long sum = 0;     /* scaled fractional part */
    unsigned long long floor_part = 0; /* floor((n-1)*log10(2)) for n-1 */

    /* Accumulate (n-1) times log10(2) in scaled integer form.
       Whenever the scaled sum reaches or exceeds SCALE, an
       additional integer is added to the floor part.
    */
    for (idx_t i = 2; i <= n; ++i) {
        sum += LOG2_SCALED;
        if (sum >= SCALE) {
            sum -= SCALE;
            ++floor_part;
        }
    }

    /* a(n) = floor_part + 1 for n ≥ 2 */
    return (seq_t)(floor_part + 1);
}
