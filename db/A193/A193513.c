/*
 * A193513  Number of partitions of n into parts having at least one common
 *          digit in decimal representation.
 * https://oeis.org/A193513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{∅≠S⊆{0…9}} (-1)^{|S|+1} p_S(n), where p_S(n) is the
 *            number of partitions of n into parts whose decimal digits all
 *            contain every digit in S. The implementation uses
 *            inclusion–exclusion over the ten decimal digits
 * Range:     u8 n <= 43, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} part_list_t;

/* Append a value to a dynamic integer array */
static void append_part(part_list_t *lst, int v)
{
    if (lst->len == lst->cap) {
        size_t newcap = lst->cap ? lst->cap * 2 : 8;
        int *newdata = (int *)realloc(lst->data, newcap * sizeof(int));
        if (!newdata) {  /* allocation failed – abort */
            free(lst->data);
            exit(EXIT_FAILURE);
        }
        lst->data = newdata;
        lst->cap = newcap;
    }
    lst->data[lst->len++] = v;
}

seq_t a(idx_t n)
{
    /* n = 0 is a special case: the empty partition is counted once */
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;

    /* Digit masks for 1 … N */
    uint16_t *digitmask = (uint16_t *)malloc((N + 1) * sizeof(uint16_t));
    if (!digitmask)
        return (seq_t)0;
    for (size_t v = 1; v <= N; ++v) {
        uint16_t m = 0;
        size_t x = v;
        while (x > 0) {
            m |= 1U << (x % 10);
            x /= 10;
        }
        digitmask[v] = m;
    }

    /* For every non‑empty digit subset, store the parts that belong to it. */
    part_list_t plist[1024];
    for (int i = 0; i < 1024; ++i) {
        plist[i].data = NULL;
        plist[i].len  = 0;
        plist[i].cap  = 0;
    }

    for (size_t v = 1; v <= N; ++v) {
        uint16_t mask = digitmask[v];
        uint16_t sub = mask;
        while (sub) {
            append_part(&plist[sub], (int)v);
            sub = (sub - 1) & mask; /* iterate over non‑zero subsets of mask */
        }
    }

    /* DP buffer for a single subset */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) {
        free(digitmask);
        return (seq_t)0;
    }

    seq_t sum_odd  = (seq_t)0;
    seq_t sum_even = (seq_t)0;

    /* Process each non‑empty subset S of digits */
    for (int S = 1; S < 1024; ++S) {
        part_list_t *lst = &plist[S];
        if (lst->len == 0)
            continue;                          /* no valid parts for this S */

        dp[0] = (seq_t)1;
        for (size_t s = 1; s <= N; ++s)
            dp[s] = (seq_t)0;

        for (size_t idx = 0; idx < lst->len; ++idx) {
            int part = lst->data[idx];
            for (size_t s = part; s <= N; ++s)
                dp[s] += dp[s - part];
        }

        seq_t fS = dp[N];
        /* Count odd or even subsets */
        int bits = 0, tmp = S;
        while (tmp) { bits++; tmp &= tmp - 1; }
        if (bits & 1)
            sum_odd += fS;
        else
            sum_even += fS;
    }

    /* Clean up */
    for (int S = 1; S < 1024; ++S)
        free(plist[S].data);
    free(digitmask);
    free(dp);

    /* Since sum_odd ≥ sum_even, the subtraction is safe. */
    seq_t result = sum_odd;
    result -= sum_even;
    return result;
}
