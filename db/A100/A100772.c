/*
 * A100772  a(1) = 1; for n>1, a(n+1) is the least number such that there are
 *          a(n) palindromes between a(n) and a(n+1) with both ends excluded.
 * https://oeis.org/A100772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n+1) = P(f(a(n)) + a(n)) + 1 where f(x) = number of
 *            palindromes ≤ x and P(k) = the k‑th decimal palindrome
 *            (1‑indexed)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_to_dec_string.h"

/* number of palindromes of a given decimal length l (l >= 1) */
static seq_t count_len(int l)
{
    /* first half length k = ceil(l/2)  */
    int k = (l + 1) / 2;
    seq_t p = pow10((idx_t)(k - 1));   /* 10^(k-1) */
    return (seq_t)9 * p;               /* 9 choices for first digit */
}

/* number of palindromes ≤ x */
static seq_t count_palindromes_leq(seq_t x)
{
    idx_t len = digit_len(x);
    seq_t total = 0;

    for (idx_t l = 1; l < len; ++l)
        total += count_len((int)l);

    /* get decimal representation of x */
    size_t buflen = (size_t)len + 1;
    char *x_str = (char*)malloc(buflen);
    seq_to_dec_string(x, x_str, buflen);

    idx_t half_len = (len + 1) / 2;

    /* prefix of length half_len */
    char *prefix = (char*)malloc(half_len + 1);
    memcpy(prefix, x_str, half_len);
    prefix[half_len] = '\0';

    /* value of the prefix */
    seq_t prefix_val = 0;
    for (idx_t i = 0; i < half_len; ++i)
        prefix_val = prefix_val * 10 + (seq_t)(prefix[i] - '0');

    seq_t min_prefix_val = pow10((idx_t)(half_len - 1));
    seq_t prefixes_less = prefix_val - min_prefix_val;

    /* build palindrome from the prefix */
    char *pal = (char*)malloc(len + 1);
    memcpy(pal, prefix, half_len);

    if (len % 2 == 0) {
        for (idx_t i = 0; i < half_len; ++i)
            pal[half_len + i] = prefix[half_len - 1 - i];
    } else {
        for (idx_t i = 0; i < half_len - 1; ++i)
            pal[half_len + i] = prefix[half_len - 2 - i];
    }
    pal[len] = '\0';

    if (strcmp(pal, x_str) <= 0)
        prefixes_less += 1;

    total += prefixes_less;

    free(x_str);
    free(prefix);
    free(pal);
    return total;
}

/* k-th palindrome (k >= 1, 1‑indexed) */
static seq_t palindrome_by_rank(seq_t rank)
{
    /* find the length that contains the rank */
    idx_t len = 1;
    seq_t cumulative = 0;

    while (1) {
        seq_t cnt = count_len((int)len);
        if (rank <= cumulative + cnt)
            break;
        cumulative += cnt;
        ++len;
    }

    /* offset within this length, 0‑based */
    seq_t offset = rank - cumulative - 1;

    idx_t half_len = (len + 1) / 2;
    seq_t min_prefix = pow10((idx_t)(half_len - 1));
    seq_t prefix_val = min_prefix + offset;  /* seq_t */

    /* convert prefix to string */
    size_t prefix_buflen = (size_t)half_len + 1;
    char *prefix = (char*)malloc(prefix_buflen);
    seq_to_dec_string(prefix_val, prefix, prefix_buflen);

    /* build palindrome string */
    size_t pal_buflen = (size_t)len + 1;
    char *pal = (char*)malloc(pal_buflen);
    memcpy(pal, prefix, half_len);

    if (len % 2 == 0) {
        for (idx_t i = 0; i < half_len; ++i)
            pal[half_len + i] = prefix[half_len - 1 - i];
    } else {
        for (idx_t i = 0; i < half_len - 1; ++i)
            pal[half_len + i] = prefix[half_len - 2 - i];
    }
    pal[len] = '\0';

    /* convert palindrome string back to seq_t */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i)
        value = value * 10 + (seq_t)(pal[i] - '0');

    free(prefix);
    free(pal);
    return value;
}

/* memoised sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    static seq_t *memo = NULL;
    static idx_t cap = 0;
    static idx_t sz  = 0;

    if (n > sz) {
        /* grow memo array */
        if (cap < n) {
            idx_t new_cap = n + 32;
            seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            memo = tmp;
            cap = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = sz; i < n; ++i) {
            idx_t idx = i + 1;   /* 1‑based index */
            if (idx == 1) {
                memo[i] = (seq_t)1;
            } else {
                seq_t prev = memo[i - 1];
                seq_t f_prev = count_palindromes_leq(prev);
                seq_t target = f_prev + prev;
                seq_t pal = palindrome_by_rank(target);
                memo[i] = pal + (seq_t)1;
            }
        }
        sz = n;
    }
    return memo[n - 1];
}
