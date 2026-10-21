/*
 * A082273  Palindromes arising in A082272, or 0 if A082272(n) is 0.
 * https://oeis.org/A082273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A082272(n) + n * [A082272(n) != 0] where A082272(n) is
 *            the smallest palindrome p such that n+p is also a palindrome, or
 *            0 if no such p exists. a(n) thus equals n+p if a palindrome p
 *            exists, otherwise 0
 * Range:     u8 none, u16 none, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"

#define MAX_PAL_LIMIT 10000000u  /* upper bound for palindromes searched */

/* helper: 10^e in seq_t */
static seq_t pow10_seq(unsigned int e)
{
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* precompute all palindromes <= MAX_PAL_LIMIT */
static seq_t *pal_array = NULL;
static size_t pal_count = 0;

/* reverse decimal digits of a small integer (fits in seq_t but < 2^64) */
static seq_t rev_digits(seq_t v)
{
    seq_t rev = 0;
    while (v > 0) {
        rev = rev * 10 + v % 10;
        v /= 10;
    }
    return rev;
}

static void init_palindromes(void)
{
    if (pal_array) return;   /* already initialized */

    /* approximate upper bound on number of palindromes <= MAX_PAL_LIMIT */
    /* length 1-7: 9 + 9 + 90 + 90 + 900 + 900 + 9000 = 10898 */
    size_t alloc_size = 20000;
    pal_array = (seq_t*)malloc(sizeof(seq_t) * alloc_size);
    pal_count = 0;

    /* generate palindromes by length */
    for (int len = 1; ; ++len) {
        int half = (len + 1) / 2;
        seq_t start = pow10_seq(half - 1);
        seq_t end   = pow10_seq(half) - 1;

        /* smallest palindrome of this length */
        seq_t firstHalf = start;
        seq_t min_pal;
        if (len % 2 == 0) {
            min_pal = firstHalf * pow10_seq(half) + rev_digits(firstHalf);
        } else {
            min_pal = firstHalf * pow10_seq(half - 1) + rev_digits(firstHalf / 10);
        }
        if (min_pal > MAX_PAL_LIMIT) break;  /* further lengths will be larger */

        for (seq_t fh = start; fh <= end; ++fh) {
            seq_t pal;
            if (len % 2 == 0) {
                pal = fh * pow10_seq(half) + rev_digits(fh);
            } else {
                pal = fh * pow10_seq(half - 1) + rev_digits(fh / 10);
            }
            if (pal > MAX_PAL_LIMIT) break;
            if (pal_count >= alloc_size) {
                alloc_size *= 2;
                pal_array = (seq_t*)realloc(pal_array, sizeof(seq_t) * alloc_size);
            }
            pal_array[pal_count++] = pal;
        }
    }
}

seq_t a(idx_t n)
{
    init_palindromes();

    seq_t nn = (seq_t)n;
    for (size_t i = 0; i < pal_count; ++i) {
        seq_t p = pal_array[i];
        seq_t sum = nn + p;
        if (is_palindrome(sum, 10)) {
            return sum;      /* a(n) = n + p */
        }
    }
    /* no suitable palindrome found within limit */
    return 0;
}
