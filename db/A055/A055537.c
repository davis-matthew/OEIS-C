/*
 * A055537  Number of asymmetric types of (4,n)-hypergraphs under action of
 *          symmetric group S_4.
 * https://oeis.org/A055537
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff_{x^n} ( (1/(1-x)^16 * - 6/(1-x)^8/(1-x^2)^4 * -
 *            3/(1-x)^4/(1-x^2)^6 * + 8/(1-x)^4/(1-x^3)^4 * +
 *            2/(1-x)^2/(1-x^2)^3/(1-x^4)^2 * + 6/(1-x)^4/(1-x^2)^4/(1-x^4) *
 *            - 8/(1-x)^2/(1-x^4)^2/(1-x^6) ) / 24 )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 3)   /* offset 3: values below are irrelevant */
        return 0;

    seq_t total = binom(n + 15, 15);          /* (1-x)^-16 */

    /* -6/(1-x)^8/(1-x^2)^4 */
    {
        seq_t sum = 0;
        for (idx_t t2 = 0; 2 * t2 <= n; ++t2) {
            idx_t r = n - 2 * t2;
            seq_t c1 = binom(r + 7, 7);     /* (1-x)^-8  */
            seq_t c2 = binom(t2 + 3, 3);    /* (1-x^2)^-4 */
            sum += c1 * c2;
        }
        total -= 6 * sum;
    }

    /* -3/(1-x)^4/(1-x^2)^6 */
    {
        seq_t sum = 0;
        for (idx_t t2 = 0; 2 * t2 <= n; ++t2) {
            idx_t r = n - 2 * t2;
            seq_t c1 = binom(r + 3, 3);     /* (1-x)^-4  */
            seq_t c2 = binom(t2 + 5, 5);    /* (1-x^2)^-6 */
            sum += c1 * c2;
        }
        total -= 3 * sum;
    }

    /* +8/(1-x)^4/(1-x^3)^4 */
    {
        seq_t sum = 0;
        for (idx_t t3 = 0; 3 * t3 <= n; ++t3) {
            idx_t r = n - 3 * t3;
            seq_t c1 = binom(r + 3, 3);     /* (1-x)^-4  */
            seq_t c2 = binom(t3 + 3, 3);    /* (1-x^3)^-4 */
            sum += c1 * c2;
        }
        total += 8 * sum;
    }

    /* +2/(1-x)^2/(1-x^2)^3/(1-x^4)^2 */
    {
        seq_t sum = 0;
        for (idx_t t2 = 0; 2 * t2 <= n; ++t2) {
            idx_t r2 = n - 2 * t2;
            for (idx_t t4 = 0; 4 * t4 <= r2; ++t4) {
                idx_t r = r2 - 4 * t4;
                seq_t c1 = binom(r + 1, 1);     /* (1-x)^-2  */
                seq_t c2 = binom(t2 + 2, 2);    /* (1-x^2)^-3 */
                seq_t c3 = binom(t4 + 1, 1);    /* (1-x^4)^-2 */
                sum += c1 * c2 * c3;
            }
        }
        total += 2 * sum;
    }

    /* +6/(1-x)^4/(1-x^2)^4/(1-x^4) */
    {
        seq_t sum = 0;
        for (idx_t t2 = 0; 2 * t2 <= n; ++t2) {
            idx_t r2 = n - 2 * t2;
            for (idx_t t4 = 0; 4 * t4 <= r2; ++t4) {
                idx_t r = r2 - 4 * t4;
                seq_t c1 = binom(r + 3, 3);     /* (1-x)^-4  */
                seq_t c2 = binom(t2 + 3, 3);    /* (1-x^2)^-4 */
                sum += c1 * c2;                 /* (1-x^4)^-1 contributes 1 */
            }
        }
        total += 6 * sum;
    }

    /* -8/(1-x)^2/(1-x^4)^2/(1-x^6) */
    {
        seq_t sum = 0;
        for (idx_t t4 = 0; 4 * t4 <= n; ++t4) {
            idx_t r2 = n - 4 * t4;
            for (idx_t t6 = 0; 6 * t6 <= r2; ++t6) {
                idx_t r = r2 - 6 * t6;
                seq_t c1 = binom(r + 1, 1);     /* (1-x)^-2  */
                seq_t c2 = binom(t4 + 1, 1);    /* (1-x^4)^-2 */
                sum += c1 * c2;                 /* (1-x^6)^-1 contributes 1 */
            }
        }
        total -= 8 * sum;
    }

    return total / 24;   /* divide by |S_4| = 24 */
}
