/*
 * A138568  Erroneous duplicate of A136094.
 * https://oeis.org/A138568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is built by concatenating the decimal digits of the
 *            sequence: 1) the numbers n, n-1, …, 1 (descending block) 2) then
 *            repeatedly: ascending block: 2, 3, …, n descending block: n-1,
 *            n-2, …, 1 The process stops after the last ascending block if n
 *            is even, and after the last descending block if...
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 4, i128 n <= 6,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only the facilities allowed in the specification. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

static unsigned digits_of_uint64(uint64_t v)
{
    unsigned d = 0;
    do {
        ++d;
        v /= 10;
    } while (v != 0);
    return d;
}

static void append_number(char *buf, size_t *pos, size_t cap, uint64_t v)
{
    /* Convert v to decimal digits and append to buf[pos].  cap is the
       total buffer size (so we never exceed it).  This function assumes
       that cap is large enough to hold the resulting digits. */
    char tmp[32];
    int len = 0;
    /* Produce digits in reverse order. */
    do {
        tmp[len++] = (char)('0' + (v % 10));
        v /= 10;
    } while (v != 0);
    /* Copy them in correct order. */
    for (int i = len - 1; i >= 0; --i) {
        buf[(*pos)++] = tmp[i];
    }
}

seq_t a(idx_t n)
{
    /* Handle the trivial case.  The sequence is defined from n>=1. */
    if (n == 0) {
        /* Undefined for n < 1; return zero for safety. */
        return 0;
    }

    /* Compute how many times each type of block appears. */
    uint64_t m = (n > 0) ? n - 1 : 0;
    size_t numDesc = m / 2;                 /* # of full descending blocks (n-1..1) */
    size_t numAsc  = (n & 1U ? numDesc : numDesc + 1); /* # of ascending blocks (2..n) */

    /* First compute the total number of digits to allocate the buffer. */
    size_t totalDigits = 0;
    for (uint64_t k = 1; k <= n; ++k) {
        unsigned cnt;
        if (k == n) {
            cnt = 1 + (unsigned)numAsc;
        } else if (k == 1) {
            cnt = 1 + (unsigned)numDesc;
        } else {
            cnt = 1 + (unsigned)numAsc + (unsigned)numDesc;
        }
        totalDigits += cnt * digits_of_uint64(k);
    }

    /* Allocate buffer big enough for all digits plus terminating NUL. */
    char *buf = (char *)malloc(totalDigits + 1);
    if (!buf) {
        /* Allocation failed – return zero to indicate failure. */
        return 0;
    }

    size_t pos = 0;  /* Current write position in buf. */

    /* 1) Descending block n, n-1, …, 1. */
    for (uint64_t i = n; i >= 1; --i) {
        append_number(buf, &pos, totalDigits, i);
        if (i == 1) break; /* prevent underflow in unsigned */
    }

    /* 2) Alternating ascending and descending blocks. */
    for (size_t block = 0; block < numAsc; ++block) {
        /* Ascending block: 2, 3, …, n. */
        for (uint64_t i = 2; i <= n; ++i) {
            append_number(buf, &pos, totalDigits, i);
        }
        /* Descending block: n-1, n-2, …, 1; only if we still have one. */
        if (block < numDesc) {
            for (uint64_t i = n - 1; i >= 1; --i) {
                append_number(buf, &pos, totalDigits, i);
                if (i == 1) break;
            }
        }
    }

    /* Null‑terminate the string. */
    buf[pos] = '\0';

    /* Convert to seq_t using the supplied utility. */
    seq_t result = seq_from_str(buf);

    /* Clean up. */
    free(buf);

    return result;
}
