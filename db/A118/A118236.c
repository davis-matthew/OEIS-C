/*
 * A118236  Numbers that can be written as sums of at least two distinct but
 *          overlapping sets of consecutive numbers.
 * https://oeis.org/A118236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that can be written as the sum * of two
 *            distinct overlapping sets of consecutive integers, * each of
 *            length ≥ 2. This implementation searches integers * sequentially
 *            and caches the values
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of computed terms. */
static seq_t *seq_cache   = NULL;   /* array holding the terms */
static idx_t  cache_len   = 0;      /* number of terms stored */
static idx_t  cache_cap   = 0;      /* capacity of seq_cache */
static idx_t  next_test   = 1;      /* next integer to test */

/* Ensure the cache can hold at least `new_cap` elements. */
static void ensure_cache_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();                     /* fatal if memory allocation fails */
    seq_cache = new_buf;
    cache_cap = new_cap;
}

/* Helper structure for a sequence: starting term `a` and length `k`. */
typedef struct { idx_t a; idx_t k; } seq_pair;

/* Static buffer reused for the list of sequences that sum to a particular
 * number.  The capacity is increased as necessary. */
static seq_pair *seqs   = NULL;
static size_t    seqs_cap = 0;

/* Test whether `num` can be expressed as the sum of two distinct
 * overlapping consecutive sequences, each of length at least two. */
static bool is_term(idx_t num)
{
    size_t len = 0;                    /* number of sequences found so far */

    /* Largest possible length `k` such that k*(k+1)/2 <= num. */
    for (idx_t k = 2; ; ++k)
    {
        idx_t min_sum = k * (k + 1) / 2;
        if (min_sum > num)
            break;
        /* Walk the k loop here (inner loops below). */
        /* Ensure `seqs` has enough capacity for at most (k) sequences. */
        if ((size_t)k > seqs_cap)
        {
            seq_pair *new_buf = (seq_pair *)realloc(seqs, (size_t)k * sizeof(seq_pair));
            if (!new_buf)
                abort();
            seqs = new_buf;
            seqs_cap = (size_t)k;
        }
        /* For the current `k`, compute sum of k consecutive numbers starting at `a`. */
        idx_t num_k = k * (k - 1) / 2;          /* sum of first (k-1) ints */
        idx_t numerator = num - num_k;
        if (numerator % k != 0)
            continue;
        idx_t a = numerator / k;
        if (a == 0)                            /* sequence must start at ≥ 1 */
            continue;

        /* Check whether this sequence overlaps any previously found sequence. */
        for (size_t j = 0; j < len; ++j)
        {
            idx_t b = seqs[j].a;
            idx_t m = seqs[j].k;
            /* Overlap iff ranges intersect. */
            if (a <= b + m - 1 && b <= a + k - 1)
                return true;
        }

        /* Store the current sequence for future overlap checks. */
        seqs[len].a = a;
        seqs[len].k = k;
        ++len;
    }

    return false;
}

/* Public accessor: return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset; placeholder value */

    /* Extend the cache until it contains at least `n` terms. */
    while (cache_len < n)
    {
        if (is_term(next_test))
        {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len] = (seq_t)next_test;
            ++cache_len;
        }
        ++next_test;
    }

    return seq_cache[n - 1];
}
