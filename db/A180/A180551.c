/*
 * A180551  Primes that become a different prime under the mapping 5 <=> 6.
 * https://oeis.org/A180551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime that when all digits 5 and 6 are swapped
 *            (5 ↔ 6) yields a different prime. Computed by a simple
 *            sieve-free primality test on successive integers
 * Range:     u8 n <= 2, u16 n <= 486, u32 n <= 579, u64 n <= 2080,
 *            u128 n <= 2080, big any n
 * Helpers:   is_prime_big, swap56
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/swap56.h"

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* safety for indices < 1 */

    /* Persisted state across calls */
    static idx_t next = 2;          /* next integer to test for primality   */
    static seq_t *seq = NULL;       /* array of found terms                 */
    static size_t cap = 0;          /* capacity of seq array                */
    static size_t len = 0;          /* number of terms stored               */

    while (len < n) {
        seq_t cur = (seq_t)next;
        if (is_prime_big(cur)) {
            seq_t trans = swap56(cur);
            if (trans != cur && is_prime_big(trans)) {
                if (len == cap) {
                    cap = (cap == 0) ? 16 : cap * 2;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                }
                seq[len++] = cur;
            }
        }
        ++next;
    }
    return seq[n - 1];
}
