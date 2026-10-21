/*
 * A235694  Semiprimes which have one or more occurrences of exactly six
 *          different digits.
 * https://oeis.org/A235694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   _smallest_prime_factor, digit_counts, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235694:  Semiprimes which have one or more occurrences of exactly six different digits
 *  Implementation strategy:  iterate through integers, test semiprime and digit distinctness,
 *  store found terms in a static array.  The array is sized generously (200 000) to handle
 *  all n ≤ 10 000 within 2 s. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/digit_counts.h"

/*  Helper: true iff value is a semiprime (product of two primes) */
static bool is_semiprime_u64(uint64_t v)
{
    if (v < 4) return false;                      /* smallest semiprime is 2*2 */
    uint64_t spf = _smallest_prime_factor(v);
    if (spf == 0 || spf == v) return false;       /* prime or composite with >2 prime factors */
    uint64_t q = v / spf;
    return is_prime_small(q);
}

/*  Storage for sequence values.  The array is oversized to avoid reallocation. */
#define SEQ_CAPACITY 200000
static seq_t seq[SEQ_CAPACITY];
static size_t seq_len = 0;

/*  Find the n-th term, computing earlier terms as needed.  Caller may call a() in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n < 1 */

    /*  Static state: next candidate to test.  Initialise at 2 (1 is not semiprime). */
    static uint64_t next_candidate = 1;

    /*  Compute terms until we have the n-th.  Assumes SEQ_CAPACITY is sufficient for the requested n. */
    while (seq_len < (size_t)n) {
        uint64_t cand = ++next_candidate;
        if (!is_semiprime_u64(cand)) continue;   /* not a semiprime */

        unsigned char cnt[10];
        digit_counts((seq_t)cand, cnt);
        int distinct = 0;
        for (int d = 0; d < 10; ++d) if (cnt[d]) distinct++;
        if (distinct != 6) continue;              /* not the required digit pattern */

        /*  store the found term */
        seq[seq_len++] = (seq_t)cand;
    }

    return seq[n - 1];
}
