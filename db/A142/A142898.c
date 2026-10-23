/*
 * A142898  Primes congruent to 17 mod 63.
 * https://oeis.org/A142898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p with p ≡ 17 (mod 63)
 * Range:     u8 n <= 1, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety for off‑by‑one calls */

    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t plen = 0, pcap = 0;

    /* static index of next k to test (k starts at 0 for candidate 17) */
    static idx_t next_k = 0;

    /* ensure we have enough primes */
    while (plen < (size_t)n)
    {
        uint64_t candidate = 17ULL + 63ULL * next_k;
        if (is_prime(candidate))
        {
            if (plen == pcap)    /* grow array */
            {
                size_t new_cap = pcap == 0 ? 16 : pcap << 1;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – return 0 as safe value */
                    return (seq_t)0;
                }
                primes = tmp;
                pcap = new_cap;
            }
            primes[plen++] = (seq_t)candidate;
        }
        ++next_k;
    }

    return primes[n - 1];
}
