/*
 * A157943  Position of 'one' in the list of names of numbers from 1 to n,
 *          sorted in English lexicographic order.
 * https://oeis.org/A157943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, look
 * Formula:   a(n) = 1 + (# of i in [1..n] such that name(i) is
 *            lexicographically less than "one")
 * Range:     u8 n <= 804, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const char *SMALL_NAMES[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *TENS_NAMES[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

static inline void append_str(char **p, const char *s)
{
    while (*s) {
        **p = *s;
        (*p)++;
        s++;
    }
}

static void int_to_name(idx_t n, char *out)
{
    char *p = out;
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        n %= 1000;
        int_to_name(thousands, p);
        while (*p) p++;
        append_str(&p, "thousand");
        if (n) {
            int_to_name(n, p);
        }
        *p = '\0';
        return;
    }
    if (n >= 100) {
        idx_t hundreds = n / 100;
        n %= 100;
        int_to_name(hundreds, p);
        while (*p) p++;
        append_str(&p, "hundred");
        if (n) {
            int_to_name(n, p);
        }
        *p = '\0';
        return;
    }
    if (n >= 20) {
        idx_t tens = n / 10;
        n %= 10;
        const char *tname = TENS_NAMES[tens];
        while (*tname) {
            *p++ = *tname++;
        }
        if (n) {
            int_to_name(n, p);
        }
        *p = '\0';
        return;
    }
    const char *sname = SMALL_NAMES[n];
    while (*sname) {
        *p++ = *sname++;
    }
    *p = '\0';
}

seq_t a(idx_t n)
{
    static idx_t max_cached = 0;
    static idx_t *less_prefix = NULL;
    static idx_t cap = 0;
    static const char one_str[] = "one";

    if (cap == 0) {
        cap = 1;
        less_prefix = (idx_t*)malloc(cap * sizeof(idx_t));
        if (!less_prefix) return 0; /* allocation failed, return 0 */
        less_prefix[0] = 0;
    }
    if (n > max_cached) {
        /* enlarge storage if needed */
        if (n + 1 > cap) {
            idx_t new_cap = n + 1;
            idx_t *tmp = (idx_t*)realloc(less_prefix, new_cap * sizeof(idx_t));
            if (!tmp) return 0; /* allocation failed; return 0 */
            less_prefix = tmp;
            cap = new_cap;
        }
        char buffer[64];
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            int_to_name(i, buffer);
            if (strcmp(buffer, one_str) < 0) {
                less_prefix[i] = less_prefix[i-1] + 1;
            } else {
                less_prefix[i] = less_prefix[i-1];
            }
        }
        max_cached = n;
    }
    return (seq_t)(less_prefix[n] + 1);
}
