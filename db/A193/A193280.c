/*
 * A193280  Triangle read by rows: row n contains, in increasing order, all
 *          the distinct sums of distinct proper divisors of n.
 * https://oeis.org/A193280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangle read by rows where row k
 *            contains all distinct sums of distinct proper divisors of k
 * Range:     u8 n <= 5059, u16 n <= 5363, u32 n <= 5363, u64 n <= 5363,
 *            u128 n <= 5363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Append a value to a dynamic array of seq_t */
static void push_seq(seq_t **seq_p, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        *cap = (*cap) ? (*cap) * 2 : 16;
        *seq_p = (seq_t *)realloc(*seq_p, (*cap) * sizeof(seq_t));
    }
    (*seq_p)[(*len)++] = val;
}

/* Generate row k and append all its distinct sums to *seq */
static void generate_row(idx_t k,
                         seq_t **seq_p, size_t *len, size_t *cap)
{
    if (k == 1) {
        push_seq(seq_p, len, cap, (seq_t)0);
        return;
    }

    /* Collect proper divisors of k */
    idx_t divisors[256];
    size_t dcnt = 0;
    divisors[dcnt++] = 1;
    for (idx_t d = 2; d * d <= k; ++d) {
        if (k % d == 0) {
            if (d < k) divisors[dcnt++] = d;
            idx_t e = k / d;
            if (e < k && e != d) divisors[dcnt++] = e;
        }
    }

    /* Compute sum of proper divisors to bound the bitset */
    seq_t summax = 0;
    for (size_t i = 0; i < dcnt; ++i)
        summax += divisors[i];

    size_t summax_s = (size_t)summax;
    size_t bit_len = (summax_s >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(bit_len, sizeof(uint64_t));

    #define SET_BIT(i) (bits[(i) >> 6] |= 1ULL << ((i) & 63))
    #define GET_BIT(i) ((bits[(i) >> 6] >> ((i) & 63)) & 1ULL)

    SET_BIT(0);  /* empty subset sum */

    /* DP over divisors */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        size_t d = (size_t)divisors[idx];
        if (d > summax_s) continue;
        for (size_t s = summax_s - d; ; --s) {
            if (GET_BIT(s)) SET_BIT(s + d);
            if (s == 0) break;
        }
    }

    /* Append all reachable sums except 0 in increasing order */
    for (size_t s = 1; s <= summax_s; ++s)
        if (GET_BIT(s))
            push_seq(seq_p, len, cap, (seq_t)s);

    free(bits);
    #undef SET_BIT
    #undef GET_BIT
}

/* Sequence accessor */
seq_t a(idx_t n)
{
    /* Static state to memoise rows */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_row = 1;   /* next k to generate */

    /* Generate rows until we have at least n terms */
    while (seq_len < (size_t)n) {
        generate_row(next_row, &seq, &seq_len, &seq_cap);
        ++next_row;
    }

    return seq[n - 1];
}
