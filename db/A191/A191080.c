/*
 * A191080  Primes p that have Kronecker symbol (p|78) = -1.
 * https://oeis.org/A191080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the Kronecker symbol (p|78) =
 *            -1 ((p|2)*(p|3)*(p|13) = -1). The function searches successive
 *            odd numbers, tests for primality and the symbol, keeps the
 *            qualifying primes in a static array, and returns the requested
 *            index
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t candidate = 5;           /* first odd prime > 3 */
    static const size_t INIT_CAP = 50000; /* enough for n up to ~100000 */

    /* initialise on first call */
    if (!primes) {
        primes = seq_alloc_array(INIT_CAP);
        if (!primes) return 0;          /* allocation failed */
    }

    /* generate terms until we have at least n */
    while (primes_cnt < (size_t)n) {
        /* primality test */
        if (is_prime_big(candidate)) {
            /* skip primes dividing 78 */
            if (candidate != 3 && candidate != 13) {
                /* (candidate|2) : 1 if candidate ≡ ±1 (mod 8) else -1 */
                int r8 = (int)(candidate % 8);
                int sign2 = (r8 == 1 || r8 == 7) ? 1 : -1;

                /* (candidate|3) : 1 if candidate ≡ 1 (mod 3) else -1 */
                int r3 = (int)(candidate % 3);
                int sign3 = (r3 == 1) ? 1 : -1;

                /* (candidate|13) : 1 if residue is in {1,3,4,9,10,12} else -1 */
                int r13 = (int)(candidate % 13);
                bool is_res13 =
                    r13 == 1 || r13 == 3 || r13 == 4 ||
                    r13 == 9 || r13 ==10 || r13 ==12;
                int sign13 = is_res13 ? 1 : -1;

                if (sign2 * sign3 * sign13 == -1) {
                    primes[primes_cnt++] = candidate;
                }
            }
        }
        candidate += 2; /* only odd candidates after 3 */
    }

    return primes[n - 1];
}
