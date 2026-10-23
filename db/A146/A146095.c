/*
 * A146095  Bell numbers (A000110) read mod 5.
 * https://oeis.org/A146095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell_n mod 5 (periodic with period 781)
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Vectors and recurrence:
   B(0) = 1
   B(n+1) = sum_{k=0}^{n} C(n,k) * B(k) (mod 5)
   and a(n) = B(n) with period 781. */
#include <stdbool.h>

/* Small binomial coefficients modulo 5 for digits 0..4.
   smC[d1][d2] = C(d1,d2) mod 5, 0 if d2 > d1. */
static const int smC[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,2,1,0,0},
    {1,3,3,1,0},
    {1,4,1,4,1}
};

#define PERIOD 781

static seq_t bell[PERIOD];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        bell[0] = 1;
        for (idx_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            idx_t m = i - 1;
            for (idx_t k = 0; k <= m; ++k) {
                seq_t c = 1;
                idx_t nn = m;
                idx_t kk = k;
                /* Lucas theorem modulo 5 */
                while (nn > 0 || kk > 0) {
                    unsigned int d1 = nn % 5;
                    unsigned int d2 = kk % 5;
                    if (d2 > d1) {
                        c = 0;
                        break;
                    }
                    c = (c * (seq_t)smC[d1][d2]) % 5;
                    nn /= 5;
                    kk /= 5;
                }
                sum += c * bell[k];
                sum %= 5;
            }
            bell[i] = sum;
        }
        initialized = true;
    }
    return bell[n % PERIOD];
}
