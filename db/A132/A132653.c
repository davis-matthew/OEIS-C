/*
 * A132653  Isolated prime I(n) raised to power I(n).
 * https://oeis.org/A132653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = I(n)^I(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where I(n) is the n-th isolated prime (prime p with no twin) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  We guard against out‑of‑range
       indices but the caller is expected to use positive indices.  */
    if (n == 0) return 0;

    /* Static state: array of isolated primes, current candidate and
       array bookkeeping.  */
    static uint64_t *isolated = NULL;
    static size_t cap = 0, count = 0;
    static uint64_t cur = 3;   /* next candidate after the first isolated prime 2 */
    static bool initialized = false;

    if (!initialized) {
        /* initial allocation for the isolated primes array */
        cap = 64;
        isolated = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!isolated) return 0;        /* out of memory – abort */
        isolated[0] = 2;                /* 2 is isolated (no 0 or 4 are primes) */
        count = 1;
        initialized = true;
    }

    /* generate isolated primes until we have at least n of them */
    while (count < (size_t)n) {
        /* Find the next isolated prime starting from cur */
        while (true) {
            if (is_prime(cur)) {
                bool isolated_candidate = true;
                if (is_prime(cur - 2) || is_prime(cur + 2))
                    isolated_candidate = false;
                if (isolated_candidate) {
                    /* dynamic resize if necessary */
                    if (count == cap) {
                        cap <<= 1;
                        uint64_t *tmp = (uint64_t *)realloc(isolated, cap * sizeof(uint64_t));
                        if (!tmp) return 0;  /* out of memory */
                        isolated = tmp;
                    }
                    isolated[count++] = cur;
                    cur++;                  /* advance past the prime we just used */
                    break;                  /* back to the outer loop to check count */
                }
            }
            cur++;                          /* test next integer */
        }
    }

    seq_t p = (seq_t)isolated[n - 1];
    return big_pow(p, p);
}
