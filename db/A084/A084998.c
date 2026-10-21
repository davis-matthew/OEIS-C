/*
 * A084998  Palindromes formed from the concatenation of n, sum of n and R(n),
 *          and R(n) with its leading zeros; or 0 if no such palindrome
 *          exists. R(k) is the digit reversal of k.
 * https://oeis.org/A084998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = palindrome formed by concatenation of n, n+rev(n), and
 *            rev(n) padded with leading zeros; 0 if no such palindrome exists
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 505, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, rev, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../utils/seq_from_str.h"
#include "../utils/rev.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Compute reverse and sum */
    idx_t rev_n = rev(n);
    idx_t sum   = n + rev_n;

    /* Lengths of components */
    int len_n   = digits(n);
    int len_sum = digits(sum);
    int len_rev = digits(rev_n);

    /* Buffer to hold concatenated string */
    size_t buf_len = (size_t)len_n + (size_t)len_sum + (size_t)len_n + 1;
    char *buf = (char *)malloc(buf_len);
    if (!buf) return 0;

    /* Build string: n + sum + rev padded */
    char *p = buf;
    sprintf(p, "%llu", (unsigned long long)n);
    p += len_n;
    sprintf(p, "%llu", (unsigned long long)sum);
    p += len_sum;

    /* Pad leading zeros for rev_n */
    for (int i = 0; i < len_n - len_rev; ++i)
        *p++ = '0';

    if (len_rev > 0)
        sprintf(p, "%llu", (unsigned long long)rev_n);
    p += len_rev;
    *p = '\0';

    /* Check palindrome property */
    bool ispal = true;
    size_t l = 0, r = strlen(buf) - 1;
    while (l < r) {
        if (buf[l] != buf[r]) {
            ispal = false;
            break;
        }
        ++l;
        --r;
    }

    seq_t result;
    if (ispal) {
        result = seq_from_str(buf);
    } else {
        result = 0;
    }

    free(buf);
    return result;
}
