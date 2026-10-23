/*
 * A133606  Numbers n with all digits different, such that all of its digits
 *          divide n, but none of the nonzero digits not in n divide n.
 * https://oeis.org/A133606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = the n‑th positive integer whose decimal digits are all
 *            distinct, the digit 1 occurs, every digit present divides the
 *            number, and no other non‑zero digit divides the number
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic array for the sequence */
static seq_t *valid = NULL;
static size_t valid_cnt = 0;
static size_t valid_cap = 0;
static bool initialized = false;

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x > *y) - (*x < *y);
}

/* append a value to the dynamic array */
static void add_val(seq_t v)
{
    if (valid_cnt == valid_cap) {
        size_t new_cap = valid_cap ? valid_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(valid, new_cap * sizeof(seq_t));
        if (!tmp) { valid_cap = 0; return; }
        valid = tmp;
        valid_cap = new_cap;
    }
    valid[valid_cnt++] = v;
}

/* check that all digit conditions hold for n under the given mask */
static bool check_candidate(seq_t n, int mask)
{
    for (int d = 1; d <= 9; ++d) {
        bool present = (mask >> (d - 1)) & 1;
        if (present) {
            if (n % d != 0) return false;
        } else {
            if (n % d == 0) return false;
        }
    }
    return true;
}

/* generate all permutations of the selected digits;
   cur is a uint64_t to avoid seq_t overflow. */
static void permute(int depth, int k, const int *digits,
                    bool *used, uint64_t cur, int mask)
{
    if (depth == k) {
        seq_t max_seq = (seq_t)~0;
        if (cur > (uint64_t)max_seq) return;          /* would overflow seq_t */
        seq_t val = (seq_t)cur;
        if (check_candidate(val, mask))
            add_val(val);
        return;
    }
    for (int i = 0; i < k; ++i) {
        if (!used[i]) {
            used[i] = true;
            permute(depth + 1, k, digits, used, cur * 10 + digits[i], mask);
            used[i] = false;
        }
    }
}

/* build the entire sequence once */
static void init_sequence(void)
{
    if (initialized) return;
    for (int mask = 1; mask < (1 << 9); ++mask) {
        if (!(mask & 1)) continue;              /* digit 1 must be present */
        int digits[9], k = 0;
        for (int d = 1; d <= 9; ++d)
            if (mask & (1 << (d - 1))) digits[k++] = d;
        bool used[9] = { false };
        permute(0, k, digits, used, 0, mask);
    }
    if (valid_cnt > 0)
        qsort(valid, valid_cnt, sizeof(seq_t), cmp_seq);
    initialized = true;
}

/* public sequence access */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > valid_cnt) return 0;
    return valid[n - 1];
}
