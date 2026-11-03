/*
 * A053055  Append n-th Fibonacci number to previous term, reverse alternate
 *          terms.
 * https://oeis.org/A053055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn, less
 * Formula:   a(n) = current string of Fibonacci numbers up to n with the
 *            string reversed for even n. Recurrence: S(0) = "" (world string
 *            before term n) S(n) = S(n‑1) ++ fib(n) (append decimal digits of
 *            the nth Fibonacci number) a(n) = S(n) if n is odd a(n) =
 *            reverse(S(n)) if n is even (here fib(n) returns the...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

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
    if (n < 1) {
        /* offset < 1 is unspecified; return 0 */
        return 0;
    }

    /* Initial buffer capacity for the concatenated string. */
    size_t capacity = 1024;
    char *buf = (char*)malloc(capacity);
    if (!buf) {
        return 0;
    }
    size_t cur_len = 0;   /* current number of valid digits in buf */

    /* Temporary buffer for each Fibonacci number's decimal representation. */
    /* 4096 is safely larger than the number of digits of F(10000). */
    char tmp[4096];
    size_t tmp_len;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t f = fib(i);
        /* Convert f to decimal string. */
        seq_to_str(f, tmp, &tmp_len);

        /* Ensure sufficient capacity (including space for eventual '\0'). */
        while (cur_len + tmp_len + 1 > capacity) {
            capacity = (cur_len + tmp_len + 1) * 2;
        }
        char *newbuf = (char*)realloc(buf, capacity);
        if (!newbuf) {
            free(buf);
            return 0;
        }
        buf = newbuf;

        memcpy(buf + cur_len, tmp, tmp_len);
        cur_len += tmp_len;
    }

    /* Null‑terminate the string for conversion. */
    buf[cur_len] = '\0';

    /* Reverse in place if n is even, because a(n) requires the reversed string. */
    if ((n & 1u) == 0) {
        for (size_t i = 0, j = cur_len - 1; i < j; ++i, --j) {
            char c = buf[i];
            buf[i] = buf[j];
            buf[j] = c;
        }
    }

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
