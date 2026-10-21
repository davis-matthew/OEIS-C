/*
 * A097222  Numbers n such that for some k there exist k numbers a1,a2, ...,ak
 *          that concatenations of them is equal to n and sum of them is equal
 *          to Pi(n).
 * https://oeis.org/A097222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = first integer m such that the decimal digits of m can be
 *            split into concatenated parts whose sum equals π(m). The
 *            function enumerates integers, keeps a dynamic list of primes to
 *            compute the prime‑count function, and tests the partition
 *            condition with a depth‑first search
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   can_partition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/can_partition.h"

seq_t a(idx_t n)
{
    /* static state preserved between calls */
    static idx_t cur = 2;              /* next integer to test (>=2) */
    static unsigned int pi_cnt = 0;    /* π(cur‑1) */
    static uint32_t *primes = NULL;
    static size_t prime_sz = 0, prime_cap = 0;
    static seq_t *seq = NULL;
    static size_t seq_sz = 0, seq_cap = 0;

    /* generate terms until we have at least n of them */
    while (seq_sz < (size_t)n) {
        /* update prime counter for cur */
        bool is_prime = true;
        if (cur < 2)
            is_prime = false;
        else {
            for (size_t i = 0; i < prime_sz; ++i) {
                uint32_t p = primes[i];
                if ((uint64_t)p * (uint64_t)p > cur)
                    break;
                if (cur % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }
        if (is_prime) {
            if (prime_sz == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_sz++] = (uint32_t)cur;
            ++pi_cnt;
        }

        /* convert cur to decimal string */
        char buf[32];
        int len = 0;
        unsigned long long tmp = cur;
        do {
            buf[len++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        } while (tmp);
        for (int i = 0; i < len / 2; ++i) {
            char t = buf[i];
            buf[i] = buf[len - 1 - i];
            buf[len - 1 - i] = t;
        }

        if (can_partition(buf, len, 0, pi_cnt)) {
            if (seq_sz == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_sz++] = (seq_t)cur;
        }

        ++cur;
    }
    return seq[n - 1];
}
