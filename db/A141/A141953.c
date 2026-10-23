/*
 * A141953  Primes congruent to 8 mod 27.
 * https://oeis.org/A141953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 8 (mod 27)
 * Range:     u8 n <= 3, u16 n <= 314, u32 n <= 352, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* primes found so far          */
    static size_t len    = 0;       /* number of stored primes      */
    static size_t cap    = 0;       /* allocated capacity           */
    static idx_t  cand   = 8;       /* last candidate checked       */

    if (n == 0)          /* below the OEIS offset – undefined; return 0 */
        return 0;

    while (len < n) {
        cand += 27;                      /* next integer ≡ 8 (mod 27)   */
        if (is_prime_big(cand)) {         /* is it prime?                 */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = (seq_t)cand;  /* store the prime              */
        }
    }

    return cache[n - 1];                 /* a(1) is the first stored     */
}
