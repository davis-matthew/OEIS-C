/*
 * A089388  Duplicate of A055494.
 * https://oeis.org/A089388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th integer k >= 2 such that k^2 - k + 1 is prime
 *            (search with primality test)
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static uint64_t last_k = 1;

    while ((idx_t)seq_len < n)
    {
        ++last_k;
        unsigned __int128 v = (unsigned __int128)last_k * last_k - last_k + 1;
        uint64_t v64 = (uint64_t)v;

        if (is_prime_u64(v64))
        {
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)last_k;
        }
    }

    return seq[n - 1];
}
