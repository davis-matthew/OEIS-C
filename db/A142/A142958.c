/*
 * A142958  Roman numerals containing three letters (i.e., III, VII, XII, XIV,
 *          ...).
 * https://oeis.org/A142958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence: a(n) = the n-th positive integer whose Roman numeral representation contains exactly three letters */

static seq_t roman_len(seq_t m)
{
    const int len_digit[10] = {0,1,2,3,2,1,2,3,4,2};

    seq_t thousands = m / 1000;
    seq_t rem = m % 1000;
    int r = (int)rem;
    int h = r / 100;
    int t = (r / 10) % 10;
    int u = r % 10;

    seq_t len = thousands
              + (seq_t)(len_digit[h] + len_digit[t] + len_digit[u]);

    return len;
}

seq_t a(idx_t n)
{
    /* static cache of all numbers with Roman numeral length 3 */
    static seq_t cache[4000];
    static size_t cache_len = 0;

    if (cache_len == 0) {
        /* maximum value a seq_t can represent */
        idx_t seq_t_max = (idx_t)(~(seq_t)0);
        idx_t limit = seq_t_max < (idx_t)3999 ? seq_t_max : (idx_t)3999;

        size_t pos = 0;
        for (idx_t mi = 1; mi <= limit; ++mi) {
            seq_t m = (seq_t)mi;
            if (roman_len(m) == 3) {
                cache[pos++] = m;
            }
        }
        cache_len = pos;
    }

    if (n == 0) return (seq_t)0;  /* undefined, but keep safety */

    size_t idx = (size_t)(n - 1);
    if (idx < cache_len) {
        return cache[idx];
    }
    return (seq_t)0;  /* out of range */
}
