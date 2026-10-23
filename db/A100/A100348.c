/*
 * A100348  Numbers n such that n-4^k is a prime for all k > 0 with 4^k < n.
 * https://oeis.org/A100348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer n such that n‑4^k is prime for all k>0 with 4^k < n */
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

static seq_t seq[200];
static unsigned seq_cnt = 0;

/* Check whether a candidate satisfies the prime‑difference condition. */
static bool good_candidate(uint64_t cand)
{
    uint64_t p = 4;          /* power 4^1 */
    while (p < cand) {
        uint64_t diff = cand - p;
        if (!is_prime(diff))
            return false;
        /* Avoid overflow when computing the next power of 4. */
        if (p > ULLONG_MAX / 4)
            break;
        p *= 4;
    }
    return true;
}

/* Generate the next term and append it to the table. */
static void generate_next(void)
{
    uint64_t cand;
    if (seq_cnt == 0)
        cand = 6;                 /* first known term */
    else
        cand = (uint64_t)seq[seq_cnt - 1] + 1;

    while (!good_candidate(cand))
        ++cand;
    seq[seq_cnt++] = (seq_t)cand;
}

/* External function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* index 0 is unused */
    while (seq_cnt < (unsigned)n)
        generate_next();
    return seq[n - 1];
}
