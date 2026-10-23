/*
 * A198298  Pandigital numbers (A050278) with each product of adjacent digits
 *          visible as a substring of the digits.
 * https://oeis.org/A198298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   brute‑force search over all 10‑digit pandigital numbers and
 *            check that for each adjacent pair of digits the product (0..81)
 *            appears as a contiguous substring of the number
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: compute next lexicographic permutation of array a[0..n-1].
   Returns false when the current permutation is the last one. */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

/* Build the complete sequence once. seq contains the terms, cnt
   receives the number of terms found (58). */
static void build_sequence(seq_t *seq, idx_t *cnt)
{
    int digits[10] = {0,1,2,3,4,5,6,7,8,9};
    idx_t found = 0;
    do {
        if (digits[0] == 0) continue;          /* skip numbers with leading 0 */

        /* build the 10‑digit number */
        uint64_t num = 0;
        for (int i = 0; i < 10; ++i) num = num * 10 + (uint64_t)digits[i];

        /* convert number to string for substring tests */
        char str[11];
        uint64_t t = num;
        for (int i = 9; i >= 0; --i) {
            str[i] = '0' + (t % 10);
            t /= 10;
        }
        str[10] = '\0';

        /* check all 9 adjacent products */
        bool ok = true;
        for (int i = 0; i < 9 && ok; ++i) {
            uint8_t a = (uint8_t)digits[i];
            uint8_t b = (uint8_t)digits[i + 1];
            uint8_t prod = a * b;
            if (prod < 10) {               /* single digit */
                if (!strchr(str, (int)('0' + prod))) ok = false;
            } else {                       /* two‑digit product */
                char prod_str[3];
                prod_str[0] = '0' + (prod / 10);
                prod_str[1] = '0' + (prod % 10);
                prod_str[2] = '\0';
                if (!strstr(str, prod_str)) ok = false;
            }
        }

        if (ok) {
            seq[found++] = (seq_t)num;
            if (found >= 60) break; /* safeguard against overflow */
        }
    } while (next_permutation(digits, 10));

    *cnt = found;
}

/* External interface matching the build system. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[60];
    static idx_t total = 0;

    if (!initialized) {
        build_sequence(seq, &total);
        initialized = true;
    }

    if (n == 0 || n > total) return 0;
    return seq[n - 1];
}
