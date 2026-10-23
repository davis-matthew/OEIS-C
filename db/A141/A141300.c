/*
 * A141300  a(n) = number of distinct (nonempty) substrings in the binary
 *          representation of n that each occur exactly once.
 * https://oeis.org/A141300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9163, u16 n <= 9163, u32 n <= 9163, u64 n <= 9163,
 *            u128 n <= 9163, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = number of distinct substrings in binary representation of n that occur exactly once */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct { unsigned long long val; uint8_t len; } Substr;

static int cmp_substr(const void *p1, const void *p2)
{
    const Substr *a = (const Substr*)p1;
    const Substr *b = (const Substr*)p2;
    if (a->len != b->len)
        return (a->len > b->len) - (a->len < b->len);
    if (a->val != b->val)
        return (a->val > b->val) - (a->val < b->val);
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint8_t len = 0;
    uint64_t tmp = (uint64_t)n;
    while (tmp) { tmp >>= 1; len++; }

    Substr arr[2100];
    size_t cnt = 0;

    for (uint8_t i = 0; i < len; ++i) {
        unsigned long long val = 0;
        for (uint8_t j = i; j < len; ++j) {
            val = (val << 1) | (((uint64_t)n >> (len - 1 - j)) & 1ULL);
            arr[cnt].val = val;
            arr[cnt].len = j - i + 1;
            ++cnt;
        }
    }

    qsort(arr, cnt, sizeof(Substr), cmp_substr);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ) {
        size_t j = i + 1;
        while (j < cnt && arr[j].len == arr[i].len && arr[j].val == arr[i].val)
            ++j;
        if (j - i == 1)
            result += (seq_t)1;
        i = j;
    }

    return result;
}
