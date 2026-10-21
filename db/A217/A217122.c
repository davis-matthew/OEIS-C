/*
 * A217122  a(n) = (number of 1's in binary expansion of n)th positive number
 *          not among the previous terms.
 * https://oeis.org/A217122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (popcount(n))-th smallest positive integer that has
 *            not appeared in any earlier term of the sequence. popcount
 *            counts the number of 1's in the binary representation of n
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *seq = NULL;      /* array of computed terms, index starts at 1   */
static size_t seq_cap = 0;      /* allocated size of seq (number of slots)    */

static uint8_t *used = NULL;   /* 0 = unused, 1 = value already taken, 1‑based      */
static size_t used_cap = 0;    /* allocated size of used array (maximum value)   */

static size_t max_n = 0;        /* largest n that has been computed              */

/* ensure that the used array can store indices up to [needed] inclusive   */
static void ensure_used_capacity(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 256;
    while (new_cap < needed)
        new_cap <<= 1;
    uint8_t *new_used = (uint8_t*)realloc(used, (new_cap + 1) * sizeof(uint8_t));
    if (!new_used)
        abort();
    for (size_t i = used_cap + 1; i <= new_cap; ++i)
        new_used[i] = 0;        /* newly added entries are unused              */
    used = new_used;
    used_cap = new_cap;
}

/* ensure that the seq array can store indices up to [needed] inclusive   */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_seq = (seq_t*)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    if (!new_seq)
        abort();
    seq = new_seq;
    seq_cap = new_cap;
}

/* compute a(n) for a single index when the sequence has been built up
   to max_n < n.  The result is placed in seq[n] and returned.           */
static seq_t compute_term(idx_t n)
{
    /* popcount of n (n is uint64_t) */
    size_t k = __builtin_popcountll((unsigned long long)n);

    /* find the k-th unused positive integer */
    size_t cnt = 0;
    size_t pos = 0;
    while (cnt < k) {
        ++pos;
        if (pos > used_cap)
            ensure_used_capacity(pos);
        if (used[pos] == 0)
            ++cnt;
    }

    /* mark that value as used */
    used[pos] = 1;

    return (seq_t)pos;
}

/* main function required by the evaluation framework */
seq_t a(idx_t n)
{
    /* very first call – initialise global structures */
    if (seq == NULL) {
        seq_cap = 256;
        seq = (seq_t*)malloc((seq_cap + 1) * sizeof(seq_t));
        if (!seq)
            abort();
    }
    if (used == NULL) {
        used_cap = 256;
        used = (uint8_t*)calloc(used_cap + 1, sizeof(uint8_t));
        if (!used)
            abort();
    }

    /* generate all terms up to the requested n */
    if ((size_t)n > max_n) {
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            seq_t val = compute_term(cur);
            ensure_seq_capacity((size_t)cur);
            seq[(size_t)cur] = val;
        }
        max_n = (size_t)n;
    }

    return seq[(size_t)n];
}
