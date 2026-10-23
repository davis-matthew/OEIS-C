/*
 * A109895  Group the natural numbers so that every 2n-th group product is
 *          divisible by the single number in the next group. (1), (2,3,4,5),
 *          (6), (7,8,9,10,11), (12), (13,14,15,16,17,18,19),(20),
 *          (21,22,23,24,25,26,27),(28),... Sequence contains the single
 *          members of the odd numbered groups.
 * https://oeis.org/A109895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n ≥ 2, a(n) is the smallest integer > a(n‑1) that
 *            divides (a(n)-1)! / (a(n-1))!; equivalently a(n) divides the
 *            product of all integers from a(n‑1)+1 up to a(n)-1
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Helper: exponent of prime p in n!   (works for n of type seq_t)     */
/* --------------------------------------------------------------------- */
static inline idx_t exp_in_fact(seq_t n, uint64_t p)
{
    idx_t e = 0;
    while (n > 0) {
        n /= p;
        e += (idx_t)n;
    }
    return e;
}

/* --------------------------------------------------------------------- */
/*  Check whether y divides (y-1)! / x!  (with y > x)                   */
/* --------------------------------------------------------------------- */
static bool divides_ratio(seq_t y, seq_t x)
{
    seq_t rem = y;

    /* handle factor 2 */
    if (rem % 2 == 0) {
        int cnt = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            ++cnt;
        }
        seq_t ym1 = y - 1;
        idx_t e = exp_in_fact(ym1, 2) - exp_in_fact(x, 2);
        if (e < (idx_t)cnt)
            return false;
    }

    /* odd factors */
    uint64_t d = 3;
    while ((seq_t)d * (seq_t)d <= rem) {
        if (rem % d == 0) {
            int cnt = 0;
            while (rem % d == 0) {
                rem /= d;
                ++cnt;
            }
            seq_t ym1 = y - 1;
            idx_t e = exp_in_fact(ym1, d) - exp_in_fact(x, d);
            if (e < (idx_t)cnt)
                return false;
        }
        d += 2;
    }

    /* remaining prime factor (if any) */
    if (rem > 1) {
        /* rem should fit in 64 bits for the required range of n */
        uint64_t p = (idx_t)rem;
        seq_t ym1 = y - 1;
        idx_t e = exp_in_fact(ym1, p) - exp_in_fact(x, p);
        if (e < (idx_t)1)
            return false;
    }

    return true;
}

/* --------------------------------------------------------------------- */
/*  Find next term after current value                                   */
/* --------------------------------------------------------------------- */
static seq_t next_from(seq_t curr)
{
    for (seq_t cand = curr + 1; ; ++cand) {
        if (divides_ratio(cand, curr))
            return cand;
    }
}

/* --------------------------------------------------------------------- */
/*  Cached computation of the sequence                                   */
/* --------------------------------------------------------------------- */
static seq_t cache[20001] = {0};   /* 20001 is safe for n up to ~10000 */
static idx_t cache_size = 1;      /* cache[0] already holds a(1) */

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – behaviour for n<1 is undefined */
        return 0;

    if (n > 20000)       /* resize needed only for very large n (rare) */
        ;
    /* initialise first term */
    if (cache[0] == 0)
        cache[0] = 1;    /* a(1) = 1 */

    while (cache_size < n) {
        seq_t next = next_from(cache[cache_size - 1]);
        cache[cache_size++] = next;
    }
    return cache[n - 1];
}
