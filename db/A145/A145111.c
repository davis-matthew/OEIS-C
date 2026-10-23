/*
 * A145111  Square array A(n,k) of numbers of length n binary words with fewer
 *          than k 0-digits between any pair of consecutive 1-digits (n,k >=
 *          0), read by antidiagonals.
 * https://oeis.org/A145111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read along antidiagonals of the array A(n,k), * where
 *            A(n,k) counts binary words of length n with fewer than k * zeros
 *            between any pair of consecutive 1‑digits. The sequence is *
 *            obtained by taking A(i,j) with i+j fixed and iterating i from 0
 * Range:     u8 n <= 86, u16 n <= 247, u32 n <= 773, u64 n <= 2620,
 *            u128 n <= 9444, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute A(n,k): number of binary strings of length n with
 * fewer than k zeros between consecutive 1s. */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (n == 0)
        return (seq_t)1;

    /* for k == 0 only strings with at most one '1' are allowed */
    if (k == 0)
        return (seq_t)1 + (seq_t)n;

    /* if the forbidden pattern cannot fit, all strings are allowed */
    if (k > n) {
        seq_t res = 1;
        for (idx_t i = 0; i < n; ++i)
            res <<= 1;
        return res;
    }

    size_t states = (size_t)k + 2;      /* 0..k-1, NONE, BLOCK */
    size_t none = k;                    /* index of the “no previous 1” state */
    size_t block = k + 1;               /* index of the “blocked” state */

    seq_t *cur = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(states, sizeof(seq_t));
    if (!cur || !nxt) {
        seq_t zero = 0;
        return zero;   /* allocation failed – return 0 */
    }

    cur[none] = 1;   /* empty string starts in NONE state */

    for (idx_t step = 0; step < n; ++step) {
        /* reset next */
        for (size_t s = 0; s < states; ++s)
            nxt[s] = 0;

        for (size_t s = 0; s < states; ++s) {
            seq_t ways = cur[s];
            if (ways == 0) continue;

            if (s == none) {          /* NONE state */
                /* append 0: stay in NONE */
                nxt[none] += ways;
                /* append 1: go to zeros = 0 state */
                nxt[0] += ways;
            } else if (s == block) {  /* BLOCK state */
                /* only 0 allowed, stay BLOCK */
                nxt[block] += ways;
            } else {                  /* zeros = s (0 <= s <= k-1) */
                /* append 0 */
                if (s + 1 == k)
                    nxt[block] += ways;      /* reach BLOCK */
                else
                    nxt[s + 1] += ways;      /* increase zeros count */

                /* append 1 */
                nxt[0] += ways;              /* reset zeros after 1 */
            }
        }

        /* swap cur and nxt */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = 0;
    for (size_t s = 0; s < states; ++s)
        result += cur[s];

    free(cur);
    free(nxt);
    return result;
}

/* helper: find the antidiagonal index s such that
 * Tri(s) = s*(s+1)/2 <= n < Tri(s+1) */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* locate (i,j) pair by antidiagonal mapping */
    idx_t s = find_diagonal(n);
    idx_t before = s * (s + 1) / 2;
    idx_t offset = n - before;

    idx_t i = offset;          /* row index */
    idx_t j = s - offset;      /* column index */

    return compute_A(i, j);
}
