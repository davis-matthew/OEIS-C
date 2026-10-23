/*
 * A108721  If a(n-1)=abcde..., where a,b,c,d,e... are the digits, then
 *          a(n)=abcde...+a*bcde...+ab*cde...+abc*de...+abcd*e...+....
 * https://oeis.org/A108721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + \sum_{i=1}^{d-1} (prefix_i * suffix_i) where d
 *            is the number of decimal digits of a(n-1), prefix_i is the
 *            integer formed by the first i digits of a(n-1), and suffix_i is
 *            the integer formed by the remaining digits. Initial value: a(1)
 *            = 111
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for indices below the offset */

    /* Memoized sequence storage */
    static idx_t computed = 0;           /* number of terms already computed */
    static idx_t seq_cap = 0;            /* capacity of seq_arr */
    static seq_t *seq_arr = NULL;        /* array holding computed terms */

    /* Helper buffers for digit extraction and intermediate values */
    static size_t rev_cap = 0, ms_cap = 0, pref_cap = 0;
    static seq_t *rev = NULL;            /* digits in least‑significant‑first order */
    static seq_t *ms = NULL;             /* digits in most‑significant‑first order */
    static seq_t *prefix_arr = NULL;     /* prefix values */
    static seq_t *suffix_arr = NULL;     /* suffix values */

    /* Grow sequence array if needed */
    if (n > seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        if (newcap < (size_t)n) newcap = n;
        seq_t *newarr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
        seq_arr = newarr;
        seq_cap = newcap;
    }

    /* Compute missing terms */
    for (idx_t i = computed; i < n; ++i) {
        if (i == 0) {
            seq_arr[0] = (seq_t)111;   /* a(1) */
            continue;
        }

        seq_t prev = seq_arr[i-1];

        /* 1. Extract decimal digits of prev (least‑significant first). */
        seq_t tmp = prev;
        size_t len = 0;
        if (tmp == 0) {
            /* Should not happen with the given sequence, but safeguard. */
            seq_arr[i] = 0;
            continue;
        }
        while (tmp != 0) {
            if (len == rev_cap) {
                size_t newcap = rev_cap ? rev_cap * 2 : 16;
                rev = (seq_t *)realloc(rev, newcap * sizeof(seq_t));
                rev_cap = newcap;
            }
            rev[len++] = tmp % 10;
            tmp /= 10;
        }

        /* 2. Build an array of digits in most‑significant‑first order. */
        if (len > ms_cap) {
            ms = (seq_t *)realloc(ms, len * sizeof(seq_t));
            ms_cap = len;
        }
        for (size_t j = 0; j < len; ++j)
            ms[j] = rev[len - 1 - j];

        /* 3. Ensure prefix and suffix buffers are large enough. */
        if (len > pref_cap) {
            prefix_arr = (seq_t *)realloc(prefix_arr, len * sizeof(seq_t));
            suffix_arr = (seq_t *)realloc(suffix_arr, len * sizeof(seq_t));
            pref_cap = len;
        }

        /* 4. Compute prefix values. */
        seq_t pref_val = 0;
        for (size_t j = 0; j < len; ++j) {
            pref_val = pref_val * (seq_t)10 + ms[j];
            prefix_arr[j] = pref_val;
        }

        /* 5. Compute suffix values using a running multiplier. */
        if (len == 1) {
            suffix_arr[0] = ms[0];
        } else {
            suffix_arr[len-1] = ms[len-1];
            seq_t mult = 1;
            for (size_t j = len-2; ; --j) {
                mult *= 10;
                suffix_arr[j] = ms[j] * mult + suffix_arr[j+1];
                if (j == 0) break;
            }
        }

        /* 6. Sum the products of corresponding prefix and suffix pairs. */
        seq_t sum = 0;
        for (size_t j = 0; j + 1 < len; ++j) {
            sum += prefix_arr[j] * suffix_arr[j+1];
        }

        /* 7. Form the new term. */
        seq_arr[i] = prev + sum;
    }

    computed = n;
    return seq_arr[n-1];
}
