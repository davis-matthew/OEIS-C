/*
 * A178386  Primes p such that q*p+-Mod(p,q) are primes, for q=6.
 * https://oeis.org/A178386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 119, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence definition:
 * A178386: primes p such that 6p ± (p mod 6) are also prime (q = 6).
 * a(n) is the n‑th such prime, 1‑indexed.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *found      = NULL;
    static size_t found_cap  = 0;
    static size_t found_cnt  = 0;
    static seq_t next_candidate = 5;   /* start with the first odd number >3 */

    while (found_cnt < n) {
        seq_t p = next_candidate;          /* current candidate prime */
        next_candidate += 2;                /* advance to next odd */

        if (!is_prime_big(p))
            continue;                       /* not prime – skip */

        seq_t r  = p % 6;                   /* r = p mod 6 */
        seq_t a1 = p * 6 + r;               /* 6p + r   */
        seq_t a2 = p * 6 - r;               /* 6p - r   */

        if (is_prime_big(a1) && is_prime_big(a2)) {
            /* expand storage if required */
            if (found_cnt == found_cap)
                grow_seq_array(&found, &found_cap, found_cnt + 1);
            found[found_cnt++] = p;        /* store the qualifying prime */
        }
    }

    return found[n - 1];                    /* a(n) is 1‑based */
}
