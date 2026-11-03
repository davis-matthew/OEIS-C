/*
 * A052062  Squares containing no palindromic substring except single digits.
 * https://oeis.org/A052062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A052061(n))^2 * Computes the nth square whose decimal
 *            representation contains no * palindromic substring of length ≥ 2
 *            (leading zeros inside substrings * are allowed). * * The
 *            sequence is generated on demand; subsequent calls reuse *
 *            previously computed terms. The implementation checks ea...
 * Range:     u8 n <= 12, u16 n <= 130, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 65 terms through n = 9998 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Helper: returns true if x contains a palindrome substring of length ≥ 2 */
static bool has_palindromic_substring(seq_t x)
{
    int d = num_digits(x);
    if (d < 2)                         /* only singles cannot fail */
        return false;

    char *buf = (char*)malloc(d + 1);   /* +1 for the terminating null */
    if (!buf)
        return true;                   /* conservatively reject on OOM */
    size_t len = d;
    seq_to_str(x, buf, &len);          /* write actual string */

    for (int i = 0; i < d - 1; ++i) {
        for (int j = i + 1; j < d; ++j) {
            int l = i, r = j;
            while (l < r && buf[l] == buf[r]) {
                ++l;
                --r;
            }
            if (l >= r) {              /* palindrome found */
                free(buf);
                return true;
            }
        }
    }

    free(buf);
    return false;
}

/* Helper: true iff m² has no palindromic substring of length ≥ 2 */
static bool is_good_number(seq_t m)
{
    seq_t sq = m * m;
    return !has_palindromic_substring(sq);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined, but safe */

    /* Static state for memoisation */
    static seq_t *array = NULL;
    static idx_t array_cap = 0;
    static idx_t array_len = 0;
    static idx_t next_candidate = 0;

    /* Grow the stored sequence until we have n terms */
    while (array_len < n) {
        seq_t m = (seq_t)next_candidate++;
        if (!is_good_number(m))
            continue;                  /* skip invalid m */

        /* Ensure capacity for the square */
        if (array_len + 1 > array_cap) {
            idx_t new_cap = array_cap ? array_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, array, new_cap);
            if (!tmp)
                abort();                /* out of memory */
            array = tmp;
            array_cap = new_cap;
        }

        seq_t val = m * m;
        array[array_len++] = val;
    }

    return array[n - 1];
}
