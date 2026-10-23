/*
 * A138830  Bisection of imperfect numbers A132999.
 * https://oeis.org/A138830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the 2n‑th positive integer that is NOT a perfect number
 *            (i.e. the bisection of the sequence of imperfect numbers).
 *
 * The sequence is obtained by listing all positive integers except
 * the perfect numbers 6, 28, 496, … and then taking the even‑indexed
 * elements of that list.  For n ≥ 1 this is the smallest integer x
 * such that x – (#perfect numbers ≤ x) = 2n.
 *
 * The implementation uses the known list of perfect numbers <= 2^64
 * (there are only 8 of them) and a simple linear search to count them.
 * For n up to 10 000 the value a(n) is well below 2^64, so the
 * algorithm performs at most O(a(n)) elementary steps, which is far
 * below the 2‑second time limit.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t _perfect_numbers[] = {
    6ULL,
    28ULL,
    496ULL,
    8128ULL,
    33550336ULL,
    8589869056ULL,
    137438691328ULL,
    2305843008139952128ULL   /* 2^p-1 * 2^(p-1) for p = 31; fits in 64 bits */
};
static const size_t _perfect_count = sizeof(_perfect_numbers) / sizeof(_perfect_numbers[0]);

/* Count how many perfect numbers ≤ x.  The result is guaranteed to be
 * small enough to fit in seq_t. */
static seq_t
_count_perfect_up_to(seq_t x)
{
    seq_t cnt = 0;
    for (size_t i = 0; i < _perfect_count; ++i) {
        if ((seq_t)_perfect_numbers[i] <= x)
            ++cnt;
        else
            break;
    }
    return cnt;
}

/* Sequence a(n) as defined for OEIS A138830. */
seq_t
a(idx_t n)
{
    if (n == 0)   /* offset is 1 – this case is not used in the tests */
        return 0;

    /* target rank among imperfect numbers (we need the 2n‑th one) */
    seq_t target_rank = (seq_t)2 * n;
    seq_t x = target_rank;       /* start at 2n */
    for (;;) {
        seq_t p = _count_perfect_up_to(x);
        seq_t current_rank = x - p;
        if (current_rank == target_rank)
            return x;
        /* current_rank is always < target_rank here, so we just increase x */
        ++x;
    }
}
