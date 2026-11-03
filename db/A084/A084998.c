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
 * Validated: standard, pass, 55 terms through n = 966 (2026-11-01)
 * Helpers:   num_digits_idx, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../utils/num_digits_idx.h"
#include "../utils/reverse_digits_idx.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* Compute reverse and sum */
    idx_t rev_n = reverse_digits_idx(n);
    idx_t sum   = n + rev_n;

    /* Lengths of components */
    int len_n   = num_digits_idx(n);
    int len_sum = num_digits_idx(sum);
    int len_rev = num_digits_idx(rev_n);

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
