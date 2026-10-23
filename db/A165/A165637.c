/*
 * A165637  Primes p such that (p^2-3)/2 is not a prime number.
 * https://oeis.org/A165637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that (p^2-3)/2 is not a prime number
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 37, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *special = NULL;
    static size_t special_cap = 0;
    static size_t special_cnt = 0;
    static uint64_t next_candidate = 2;

    if (n == 0) return (seq_t)0;   /* Safety for n=0, though not used */

    while (special_cnt < (size_t)n) {
        for (;;) {
            seq_t cand_seq = (seq_t)next_candidate;
            bool cand_is_prime = is_prime_big(cand_seq);
            uint64_t cand_val = next_candidate;
            ++next_candidate;           /* advance for next iteration */

            if (!cand_is_prime) continue;

            if (cand_val == 2) {        /* include 2 explicitly */
                if (special_cnt == special_cap) {
                    size_t new_cap = special_cap ? special_cap * 2 : 8;
                    special = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
                    special_cap = new_cap;
                }
                special[special_cnt++] = (seq_t)2;
                break;
            } else {
                /* compute (p^2-3)/2 */
                seq_t expr = ((cand_seq * cand_seq - (seq_t)3) / (seq_t)2);
                if (!is_prime_big(expr)) {
                    if (special_cnt == special_cap) {
                        size_t new_cap = special_cap ? special_cap * 2 : 8;
                        special = (seq_t *)realloc(special, new_cap * sizeof(seq_t));
                        special_cap = new_cap;
                    }
                    special[special_cnt++] = cand_seq;
                    break;
                }
            }
        }
    }
    return special[(size_t)n - 1];
}
