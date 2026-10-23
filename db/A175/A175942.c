/*
 * A175942  Odd numbers k such that 4^k == 4 (mod 3*k) and 2^(k-1) == 4 (mod
 *          3*(k-1)).
 * https://oeis.org/A175942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 479, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175942: odd numbers k such that 4^k == 4 (mod 3*k) and
   2^(k-1) == 4 (mod 3*(k-1)).  Efficient generation uses
   modular exponentiation (mod_pow) and iterates k ≡ 5 (mod 6). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static state for memoised sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t count = 0;          /* number of terms stored */
    static seq_t next_candidate = 5; /* next k ≡ 5 (mod 6) to test */
    static bool started = false;

    if (!started) {
        started = true;
        next_candidate = 5;
    }

    while ((idx_t)count < n) {
        seq_t k = next_candidate;          /* current candidate */

        /* Condition 1: k | 4^k − 4   ⇔   4^k (mod k) == 4  (since 4 < k for all k>=5) */
        seq_t mod1 = mod_pow((seq_t)4, (uint64_t)k, k);
        bool ok1 = (mod1 == (seq_t)4);

        /* Condition 2: (k−1) | 2^(k−1) − 4   ⇔   2^(k−1) (mod (k−1)) == 4 (mod (k−1)) */
        seq_t k1 = k - 1;
        seq_t mod2 = mod_pow((seq_t)2, (uint64_t)k1, k1);
        seq_t rhs2 = ((seq_t)4) % k1;      /* rhs2 == 4 (mod k1) */
        bool ok2 = (mod2 == rhs2);

        if (ok1 && ok2) {
            /* append to stored sequence */
            grow_seq(&seq, &seq_cap, (size_t)count + 1);
            seq[count] = k;
            count++;
        }

        next_candidate += 6; /* advance to next k ≡ 5 (mod 6) */
    }

    return seq[n - 1];
}
