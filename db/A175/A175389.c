/*
 * A175389  Smallest nonnegative number k such that 2^k contains n, 2n and 3n
 *          as substrings of its decimal expansion.
 * https://oeis.org/A175389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that 2^k contains n, 2n, 3n as decimal
 *            substrings
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* multiply a decimal string by 2; return a newly allocated string */
static char *mul2(const char *s)
{
    size_t len = strlen(s);
    /* maximum digits: len + 1 (carry) + 1 (null) */
    char *res = (char *)malloc(len + 2);
    if (!res) return NULL;
    int carry = 0;
    int i = (int)len - 1;
    int pos = (int)len + 1;          /* leave space for null at end */
    res[pos] = '\0';
    while (i >= 0 || carry)
    {
        int val = carry;
        if (i >= 0)
        {
            val += (s[i] - '0') * 2;
            --i;
        }
        res[--pos] = (char)('0' + (val % 10));
        carry = val / 10;
    }
    /* shift to front if needed */
    /* pos now points to first digit of the result */
    char *trimmed = (char *)malloc(len + 2 - pos);
    if (!trimmed) { free(res); return NULL; }
    memcpy(trimmed, res + pos, len + 2 - pos);
    free(res);
    return trimmed;
}

/* duplicate a string (C standard does not guarantee strdup) */
static char *copy_str(const char *s)
{
    size_t len = strlen(s);
    char *p = (char *)malloc(len + 1);
    if (!p) return NULL;
    memcpy(p, s, len + 1);
    return p;
}

seq_t a(idx_t n)
{
    /* convert n, 2n, 3n to strings */
    char n_str[32], n2_str[32], n3_str[32];
    unsigned long long un = (unsigned long long)n;
    snprintf(n_str, sizeof(n_str), "%llu", un);
    snprintf(n2_str, sizeof(n2_str), "%llu", un * 2ULL);
    snprintf(n3_str, sizeof(n3_str), "%llu", un * 3ULL);

    /* current power of 2 as decimal string, starting from 2^0 = "1" */
    char *cur = copy_str("1");
    if (!cur) return (seq_t)0;              /* fallback */

    for (uint64_t k = 0;; ++k)
    {
        if (strstr(cur, n_str) && strstr(cur, n2_str) && strstr(cur, n3_str))
        {
            free(cur);
            return (seq_t)k;
        }
        char *next = mul2(cur);
        free(cur);
        if (!next) return (seq_t)0;          /* error fallback */
        cur = next;
    }
}
