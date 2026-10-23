/*
 * A188454  Numbers n whose decimal digits are distinct and no digit divides
 *          n.
 * https://oeis.org/A188454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini
 * Formula:   a(n) is the nth integer whose decimal digits are distinct,
 *            contain no 1, and none of its digits divides the integer. The
 *            implementation enumerates all permutations of the digits
 *            {0,2,3,4,5,6,7,8,9} (without leading 0), builds the
 *            corresponding numbers, keeps those that satisfy the divisibility
 *            re...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const int DIGITS[9] = {0,2,3,4,5,6,7,8,9};

static seq_t *seq_array = NULL;
static size_t seq_size = 0;

/* Compare two seq_t values for qsort. */
static int seq_compare(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Recursive enumeration of all permutations of subsets of DIGITS. */
static void generate_rec(unsigned int mask,
                         int len,
                         int cur_digits[9],
                         seq_t cur_val,
                         seq_t **arr,
                         size_t *cnt,
                         size_t *cap)
{
    for (int i = 0; i < 9; ++i) {
        if (mask & (1U << i))
            continue;
        int d = DIGITS[i];
        if (len == 0 && d == 0)  /* no leading zero */
            continue;

        cur_digits[len] = d;
        seq_t next_val = cur_val * (seq_t)10 + (seq_t)d;

        /* test divisibility by all non‑zero digits used so far */
        bool ok = true;
        for (int j = 0; j <= len; ++j) {
            int dd = cur_digits[j];
            if (dd == 0)
                continue;
            if (next_val % (seq_t)dd == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            if (*cnt == *cap) {
                *cap = (*cap == 0 ? 1024 : *cap << 1);
                *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
            }
            (*arr)[*cnt] = next_val;
            (*cnt)++;
        }

        generate_rec(mask | (1U << i), len + 1,
                     cur_digits, next_val, arr, cnt, cap);
    }
}

/* Build the entire sequence once. */
static void build_sequence(void)
{
    seq_t *cand = NULL;
    size_t cap = 0;
    size_t count = 0;
    int digits[9];

    generate_rec(0, 0, digits, (seq_t)0, &cand, &count, &cap);

    /* sort the collected numbers */
    qsort(cand, count, sizeof(seq_t), seq_compare);

    seq_array = cand;
    seq_size  = count;
}

seq_t a(idx_t n)
{
    if (!seq_array)
        build_sequence();

    if (n == 0 || n > (idx_t)seq_size)
        return (seq_t)0;  /* out of bounds – should not happen for valid queries */

    return seq_array[n - 1];
}
