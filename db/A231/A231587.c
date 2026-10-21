/*
 * A231587  Numbers k such that sigma(sigma(k)) - sigma(k) + 1 is prime.
 * https://oeis.org/A231587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest k such that sigma(sigma(k)) - sigma(k)
 *            + 1 is prime
 * Range:     u8 n <= 27, u16 n <= 1788, u32 n <= 2629, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_k = 1;

    if (n == 0) return (seq_t)0;   /* sequence is indexed from 1 */

    while (seq_len < (size_t)n) {
        idx_t k = next_k;
        seq_t sigma_k   = sigma_of(k);
        seq_t sigma_sigma = sigma_of((idx_t)sigma_k);
        seq_t target = sigma_sigma - sigma_k + 1;
        if (is_prime_big(target)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = k;
        }
        ++next_k;
    }

    return seq[n - 1];
}
