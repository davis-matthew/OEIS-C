/*
 * A199921  Number of Roman numerals < 4000 with n letters.
 * https://oeis.org/A199921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of Roman numerals < 4000 that consist of exactly n
 *            symbols. Computed by brute‑force enumeration of all values 1 …
 *            3999 and counting lengths
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* table of standard Roman numeral components, sorted from largest to smallest */
static const struct {
    int val;    /* integer value */
    int slen;   /* number of symbol characters in the representation */
} roman_table[] = {
    {1000, 1}, {900, 2}, {500, 1}, {400, 2},
    {100, 1},  {90, 2},  {50, 1},  {40, 2},
    {10, 1},   {9, 2},   {5, 1},   {4, 2},
    {1, 1}
};

static const size_t roman_table_len = sizeof(roman_table) / sizeof(roman_table[0]);

/* Count of numerals having a given length (1 … 15).  Index 0 is unused. */
static seq_t _roman_counts[16];
static bool _roman_counts_built = false;

/* Return the number of characters needed to write the Roman numeral for num (1 ≤ num ≤ 3999). */
static int roman_len(int num)
{
    int len = 0;
    for (size_t i = 0; i < roman_table_len; ++i) {
        while (num >= roman_table[i].val) {
            num -= roman_table[i].val;
            len += roman_table[i].slen;
        }
        if (num == 0)
            break;
    }
    return len;
}

/* Build the length counts once, the first time a() is called. */
static void build_counts(void)
{
    if (_roman_counts_built)
        return;

    /* initialise all counts to zero */
    for (size_t i = 0; i < 16; ++i)
        _roman_counts[i] = 0;

    for (idx_t i = 1; i <= 3999; ++i) {
        int l = roman_len((int)i);          /* length of i */
        if (l < 16)
            _roman_counts[(size_t)l] += 1;   /* increment count for that length */
    }
    _roman_counts_built = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* values below the offset are undefined, return 0 */
        return 0;
    if (n > 15)                /* maximum Roman numeral length below 4000 is 15 */
        return 0;

    if (!_roman_counts_built)
        build_counts();

    return _roman_counts[(size_t)n];
}
