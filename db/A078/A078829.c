/*
 * A078829  Numbers having exactly one prime contained as binary substring in
 *          binary representation of n.
 * https://oeis.org/A078829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078829: numbers whose binary representation contains exactly one
   distinct prime substring.  The function searches numbers in increasing order
   until the n‑th such number is found.  Duplicate prime substrings are
   counted once (e.g. 18 has the distinct prime 10 twice in binary 10010). */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"

static bool is_single_prime_binary_substring(seq_t x)
{
    unsigned L = bitlen_seq_t(x);
    /* Binary representation without leading zeros, MSB first */
    uint8_t bits[128];
    for (unsigned i = 0; i < L; ++i) {
        bits[i] = ((x >> (L - 1 - i)) & (seq_t)1) ? 1 : 0;
    }

    seq_t primes[128];   /* store distinct prime substrings */
    unsigned prime_cnt = 0;

    for (unsigned i = 0; i < L; ++i) {
        seq_t val = 0;
        for (unsigned j = i; j < L; ++j) {
            val = (val << 1) | (seq_t)bits[j];
            if (val < 2) continue;
            if (!is_prime_big(val)) continue;

            /* check for duplicates */
            bool dup = false;
            for (unsigned k = 0; k < prime_cnt; ++k) {
                if (primes[k] == val) { dup = true; break; }
            }
            if (dup) continue;

            primes[prime_cnt++] = val;
            if (prime_cnt > 1) return false; /* more than one distinct prime */
        }
    }
    return prime_cnt == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard for safety */

    seq_t candidate = 2;          /* smallest possible value in the sequence */
    idx_t found = 0;

    while (1) {
        if (is_single_prime_binary_substring(candidate)) {
            found++;
            if (found == n) return candidate;
        }
        candidate++;
    }
}
