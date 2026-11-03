/*
 * A008570  Digits of powers of 9.
 * https://oeis.org/A008570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4866, u16 n <= 4866, u32 n <= 4866, u64 n <= 4866,
 *            u128 n <= 4866, big any n
 * Validated: standard, pass, 115 terms through n = 4761 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008570: concatenated decimal digits of powers of 9
 *
 * a(n) is the (n+1)-st digit (0‑based) in the infinite string
 *   1,9,81,729,6561,59049,...
 *   obtained by writing 9^0,9^1,9^2,… in base 10 and concatenating.
 *
 * The function lazily caches all powers of 9 as decimal strings
 * together with their cumulative digit lengths.  On each call it
 * extends the cache until it contains the requested digit position.
 *
 * The implementation uses only integer arithmetic and standard
 * C library functions, working for any seq_t (uint8_t…uint64_t,
 * arbitrary‑precision) and idx_t (uint64_t).
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Helper: multiply a decimal string by 9.
 *   s   – input string, most significant digit first, no leading zeros
 *   len – length of input string
 *   out_len – receives the length of the allocated result
 * Returns a newly malloc’ed string containing the product, null terminated. */
static char *mult_by_9(const char *s, size_t len, size_t *out_len)
{
    /* A carry may add at most one digit. */
    char *tmp = (char *)malloc(len + 2);          /* +1 for carry, +1 for '\0' */
    if (!tmp) abort();

    int carry = 0;
    for (long i = (long)len - 1; i >= 0; --i)
    {
        int prod = (s[i] - '0') * 9 + carry;
        tmp[i + 1] = (char)('0' + (prod % 10));
        carry = prod / 10;
    }

    if (carry > 0)
    {
        tmp[0] = (char)('0' + carry);
        *out_len = len + 1;
        tmp[*out_len] = '\0';
    }
    else
    {
        /* Shift left to discard the leading zero. */
        memmove(tmp, tmp + 1, len);
        *out_len = len;
        tmp[*out_len] = '\0';
    }
    return tmp;
}

/* Forward declaration of the sequence function. */
seq_t a(idx_t n);

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of powers of 9 as decimal strings. */
    static char   **powers       = NULL;    /* powers[i] = 9^i as string */
    static size_t  *p_len        = NULL;    /* length of powers[i]      */
    static idx_t   *cum_digits   = NULL;    /* cumulative digit counts  */
    static size_t   capacity     = 0;       /* allocated size of arrays */
    static size_t   count        = 0;       /* number of powers stored */

    /* Lazy initialization. */
    if (count == 0)
    {
        capacity = 16;
        count = 1;
        powers = (char **)malloc(capacity * sizeof(char *));
        p_len = (size_t *)malloc(capacity * sizeof(size_t));
        cum_digits = (idx_t *)malloc(capacity * sizeof(idx_t));
        if (!powers || !p_len || !cum_digits) abort();

        /* 9^0 = 1 */
        powers[0] = (char *)malloc(2);
        if (!powers[0]) abort();
        powers[0][0] = '1';
        powers[0][1] = '\0';
        p_len[0] = 1;
        cum_digits[0] = 1;                 /* one digit so far */
    }

    /* Extend the cache until it covers the requested index n. */
    while (n >= cum_digits[count - 1])
    {
        /* Ensure space for the new entries. */
        if (count == capacity)
        {
            capacity *= 2;
            char **new_powers = (char **)realloc(powers, capacity * sizeof(char *));
            size_t *new_p_len = (size_t *)realloc(p_len, capacity * sizeof(size_t));
            idx_t *new_cum = (idx_t *)realloc(cum_digits, capacity * sizeof(idx_t));
            if (!new_powers || !new_p_len || !new_cum) abort();
            powers = new_powers;
            p_len = new_p_len;
            cum_digits = new_cum;
        }

        /* Compute 9^count from the previous power. */
        size_t next_len;
        char *next_str = mult_by_9(powers[count - 1], p_len[count - 1], &next_len);
        if (!next_str) abort();

        powers[count] = next_str;
        p_len[count] = next_len;
        cum_digits[count] = cum_digits[count - 1] + (idx_t)next_len;
        count++;
    }

    /* Find the power that contains the digit. */
    size_t power_index = count - 1;
    idx_t prev_cum = (power_index == 0) ? 0 : cum_digits[power_index - 1];
    idx_t offset = n - prev_cum;                 /* 0‑based offset within this power */
    if ((size_t)offset >= p_len[power_index]) abort();    /* safety check */

    char digit_char = powers[power_index][(size_t)offset];
    return (seq_t)(digit_char - '0');
}
