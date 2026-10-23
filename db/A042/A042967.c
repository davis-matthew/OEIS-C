/*
 * A042967  Primes p such that x^7 = 2 has no solution mod p.
 * https://oeis.org/A042967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 940, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A042967.
   Record numbers p such that the congruence x^7 ≡ 2 (mod p) has no solution.
   For odd primes p, this holds iff p ≡ 1 (mod 7) and 2^{(p−1)/7} ≠ 1 (mod p). */

#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/powmod64.h"

static seq_t *seq   = NULL;          /* array of sequence values */
static size_t seq_len = 0;           /* number of values stored */
static size_t seq_alloc = 0;         /* current allocation size */
static uint64_t last_checked = 2;    /* last candidate examined */

static inline bool is_prime_candidate(uint64_t n)
{
    return is_prime_small(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined but safe */

    /* Acquire sequence values until we have at least n of them. */
    while (seq_len < n)
    {
        /* Find the next odd prime greater than last_checked. */
        uint64_t cand = last_checked + 1;
        if ((cand & 1) == 0) ++cand;    /* make it odd */

        while (!is_prime_candidate(cand))
            cand += 2;

        /* Test the congruence condition. */
        if (cand % 7 == 1)
        {
            uint64_t exp = (cand - 1) / 7;
            uint64_t r   = powmod64(2, exp, cand);
            if (r != 1)
            {
                /* Store the prime in the sequence. */
                if (seq_len == seq_alloc)
                {
                    seq_alloc = seq_alloc ? seq_alloc * 2 : 64;
                    seq = (seq_t *)realloc(seq, seq_alloc * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)cand;
            }
        }

        last_checked = cand;          /* advance pointer for next search */
    }

    return seq[n - 1];
}
