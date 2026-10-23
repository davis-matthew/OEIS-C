/*
 * A175304  A positive integer n is included if d(n+d(n)) = d(n), where d(n)
 *          is the number of divisors of n.
 * https://oeis.org/A175304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence A175304:
   a(n) is the nth positive integer m such that
   d(m + d(m)) = d(m),
   where d(k) is the number of positive divisors of k. */

/* -------------------------------------------------------------------- */
/* Helper: divisor count for 64‑bit integers (trial division).          */
/* -------------------------------------------------------------------- */
static uint64_t divisor_count(uint64_t n)
{
    if (n <= 1) return 1;          /* d(1) = 1, d(0) not used                */
    uint64_t cnt = 1;
    uint64_t temp = n;

    /* factor 2 separately */
    if ((temp & 1u) == 0) {
        uint64_t e = 0;
        while ((temp & 1u) == 0) {
            temp >>= 1;
            ++e;
        }
        cnt *= (e + 1);
    }

    /* odd factors: trial division up to sqrt(n) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        cnt *= 2;
    }

    return cnt;
}

/* -------------------------------------------------------------------- */
/* Dynamic cache for the sequence values.                               */
/* -------------------------------------------------------------------- */
static seq_t *memo = NULL;      /* stored sequence terms (a(1)=memo[0])      */
static size_t  memo_cap = 0;    /* current memory capacity in terms          */
static idx_t   memo_cnt = 0;    /* number of terms actually stored           */
static uint64_t search_next = 1;/* next integer to test for the sequence     */

static void grow_memo(size_t min_cap)
{
    if (min_cap <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 64;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) abort();           /* out of memory */
    memo = new_mem;
    memo_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/* Public function.                                                     */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but harmless              */

    /* Expand the cache until the nth term has been found.             */
    while (memo_cnt < n) {
        uint64_t cand  = search_next;
        uint64_t d_cand = divisor_count(cand);
        uint64_t target  = cand + d_cand;
        uint64_t d_target= divisor_count(target);

        if (d_cand == d_target) {
            /* Found a new element of the sequence. */
            grow_memo(memo_cnt + 1);
            memo[memo_cnt++] = (seq_t)cand;   /* store as seq_t            */
        }
        ++search_next;   /* advance to the next candidate              */
    }

    return memo[n - 1];
}
