/*
 * A072483  Triangle read by rows: The n-th row contains the smallest n
 *          increasing numbers larger than the last term of the previous row,
 *          which contain the string of digits of n.
 * https://oeis.org/A072483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method:
   Generate the triangle row by row.  For row r, find the first r numbers
   strictly larger than the last number of the previous row that contain
   the decimal string representation of r.  Append them to a cache and
   return the n‑th term when the cache contains at least n elements. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

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

static bool seq_contains_substr(seq_t value, const char *target)
{
    /* Convert value to decimal string and test for substring. */
    char buf[256];
    size_t len;
    seq_to_str(value, buf, &len);
    buf[len] = '\0';
    return strstr(buf, target) != NULL;
}

seq_t a(idx_t n)
{
    /* Cache of all generated terms. */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0;
    static size_t capacity   = 0;

    /* State for the generic row builder. */
    static seq_t  lastNumber            = 0;   /* last value produced */
    static idx_t  curRow                = 1;   /* next row to build */
    static idx_t  numsInCurrentRow      = 0;   /* how many in current row */
    static char   targetStr[32];                /* decimal string of curRow */

    /* Grow the cache as needed. */
    if (cacheSize < n && cache == NULL) {
        capacity   = 64;
        cache      = mem_new(seq_t, capacity);
    }

    while (cacheSize < n) {
        /* When starting a new row, compute the target string. */
        if (numsInCurrentRow == 0) {
            snprintf(targetStr, sizeof(targetStr), "%llu",
                     (unsigned long long)curRow);
        }

        /* Find the next number > lastNumber that contains targetStr. */
        seq_t cand = lastNumber + 1;
        while (!seq_contains_substr(cand, targetStr)) {
            cand = cand + 1;
        }

        /* Append the found candidate. */
        if (cacheSize == capacity) {
            capacity = capacity * 2;
            cache    = mem_resize(seq_t, cache, capacity);
        }
        cache[cacheSize++] = cand;
        lastNumber = cand;
        numsInCurrentRow++;

        /* If the current row is finished, move to the next row. */
        if (numsInCurrentRow == curRow) {
            curRow++;
            numsInCurrentRow = 0;
        }
    }

    return cache[n - 1];
}
