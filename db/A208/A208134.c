/*
 * A208134  Number of zeros in n-th row of Pascal's triangle mod 10 (A008975).
 * https://oeis.org/A208134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binomial coefficients C(n,k) (k = 0..n) that
 *            are divisible by 10 (i.e. divisible by both 2 and 5)
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed binomial coefficients modulo 5 for 0 ≤ n,k ≤ 4.
   Non‑zero entries indicate that C(n,k) is not divisible by 5. */
static const unsigned char binom_mod5[5][5] = {
    /* k=0 1 2 3 4 */
    {1,0,0,0,0}, /* n=0 */
    {1,1,0,0,0}, /* n=1 */
    {1,2,1,0,0}, /* n=2 */
    {1,3,3,1,0}, /* n=3 */
    {1,4,1,4,1}  /* n=4 */
};

/* Count zeros in row n. */
seq_t a(idx_t n)
{
    seq_t zeros = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Check divisibility by 2: C(n,k) is odd iff (k & ~n) == 0. */
        bool odd = ( (k & ~n) == 0 );

        /* Check divisibility by 5 via Lucas theorem for base 5. */
        bool div5 = false;          /* true if C(n,k) divisible by 5 */
        idx_t kk = k;
        idx_t nn = n;
        while (kk > 0 || nn > 0) {
            unsigned int ki = kk % 5;
            unsigned int ni = nn % 5;
            if (ki > ni || binom_mod5[ni][ki] == 0) {
                div5 = true;
                break;
            }
            kk /= 5;
            nn /= 5;
        }

        /* C(n,k) is divisible by 10 iff it is even (not odd) and divisible by 5. */
        if (!odd && div5) {
            ++zeros;
        }
    }

    return zeros;
}
