/*
 * A197640  Numbers not representable as the sum of two coprime, squarefree,
 *          composite, positive integers.
 * https://oeis.org/A197640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th natural number that cannot be expressed as the
 *            sum of two coprime, square‑free, composite positive integers
 * Range:     u8 n <= 84, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd, is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies from the utils library */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* Upper limit for precomputation – large enough to cover the known terms (a(87)=420) */
#define LIMIT 5000

seq_t a(idx_t n)
{
    /* Cache of the sequence values */
    static seq_t seq[128];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Flag array: true ↔ number is square‑free composite */
        static bool eligible[LIMIT + 1];   /* zero‑initialized */

        /* mark all squarefree composite numbers up to LIMIT */
        for (size_t num = 4; num <= LIMIT; ++num) {
            if (!is_prime(num) && is_squarefree((seq_t)num))
                eligible[num] = true;
        }

        /* find all numbers that cannot be written as a sum of two
           coprime, squarefree, composite positive integers */
        for (size_t m = 1; m <= LIMIT; ++m) {
            bool represented = false;

            for (size_t a = 4; a < m; ++a) {
                if (!eligible[a])
                    continue;
                size_t b = m - a;
                if (b < 4 || !eligible[b])
                    continue;
                if (gcd((seq_t)a, (seq_t)b) == 1) {
                    represented = true;
                    break;
                }
            }

            if (!represented) {
                seq[count++] = (seq_t)m;
            }
        }

        initialized = true;
    }

    if (n == 0 || n > count)
        return 0;
    return seq[n - 1];
}
