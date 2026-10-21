/*
 * A066956  a(n) is the number of ways of placing '+' and '-' among the digits
 *          123456789 so that the result of the expression is n, '-' before 1
 *          not allowed.
 * https://oeis.org/A066956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8718, u16 n <= 8718, u32 n <= 8718, u64 n <= 8718,
 *            u128 n <= 8718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ways to place '+' and '-' among 123456789 (no minus before 1)
   so that the resulting signed sum equals n.  This implementation enumerates
   all 3⁸ = 6561 possible expressions and builds a lookup table. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct Entry {
    int sum;      /* signed sum of one expression */
    seq_t count;  /* number of expressions giving this sum */
};

/* static table of all possible sums */
static struct Entry table[6561];
static size_t table_len = 0;
static int initialized = 0;

/* comparison function for qsort */
static int cmp_entry(const void *a, const void *b)
{
    const struct Entry *ea = (const struct Entry *)a;
    const struct Entry *eb = (const struct Entry *)b;
    return (ea->sum > eb->sum) - (ea->sum < eb->sum);
}

/* binary search in the sorted table */
static int find_index(int target)
{
    size_t lo = 0, hi = table_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (table[mid].sum < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < table_len && table[lo].sum == target)
        return (int)lo;
    return -1;
}

/* initialize table once */
static void init_table(void)
{
    if (initialized)
        return;

    /* enumerate all 3^8 expressions */
    for (int mask = 0; mask < 6561; ++mask) {
        int rem = mask;
        int digits[8];
        for (int i = 0; i < 8; ++i) {
            digits[i] = rem % 3;  /* 0 = concat, 1 = +, 2 = - */
            rem /= 3;
        }

        int sum = 0;
        int curr = 1;           /* current number being built */
        int sign = 1;           /* always positive for the first block */

        for (int pos = 0; pos < 8; ++pos) {
            int choice = digits[pos];
            int next_digit = pos + 2; /* digits: 2..9 */

            if (choice == 0) {       /* concatenate */
                curr = curr * 10 + next_digit;
            } else {
                sum += sign * curr;   /* finish current block */
                sign = (choice == 1) ? 1 : -1;
                curr = next_digit;    /* start new block */
            }
        }
        sum += sign * curr;           /* add last block */

        /* record in table */
        int idx = -1;
        for (size_t i = 0; i < table_len; ++i) {
            if (table[i].sum == sum) {
                idx = (int)i;
                break;
            }
        }
        if (idx == -1) {
            table[table_len].sum = sum;
            table[table_len].count = 1;
            ++table_len;
        } else {
            ++table[idx].count;
        }
    }

    /* sort table by sum to enable binary search */
    qsort(table, table_len, sizeof(struct Entry), cmp_entry);

    initialized = 1;
}

seq_t a(idx_t n)
{
    init_table();

    if (n > INT32_MAX)            /* sums are within 32-bit range */
        return 0;

    int target = (int)n;
    int idx = find_index(target);
    if (idx == -1)
        return 0;
    return table[idx].count;
}
