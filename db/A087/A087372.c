/*
 * A087372  Smallest k such that 3^k + 2n is a prime, or 0 if no such k
 *          exists. Index of the smallest power of 3 which when increased by
 *          the n-th even number yields a prime.
 * https://oeis.org/A087372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k such that 3^k + 2*n is a prime.
   Return 0 if no such k exists. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset 1, guard for n<1 */

    /* If n ≡ 0 (mod 3) then 2*n is a multiple of 3.
       Every candidate 3^k + 2*n (k≥1) is then divisible by 3 and >3,
       so it is composite.  Thus a(n)=0 for n divisible by 3. */
    if ((n % 3) == 0) return 0;

    seq_t twoN = (seq_t)n * 2;

    seq_t pow3 = 1;                      /* hold 3^k */
    const idx_t MAX_K = 2000;            /* empirical bound for the search */

    for (idx_t k = 1; k <= MAX_K; ++k) {
        pow3 *= (seq_t)3;                /* update to 3^k */
        seq_t candidate = pow3 + twoN;   /* 3^k + 2*n */
        if (is_prime_big(candidate)) return (seq_t)k;
    }
    return 0;                            /* no prime within the bound */
}
