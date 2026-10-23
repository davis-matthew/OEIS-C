/*
 * A141297  a(n) = number of distinct (nonempty) substrings in the binary
 *          representation of n.
 * https://oeis.org/A141297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = number of distinct non‑empty substrings of the binary *
 *            representation of n (substrating may start with a 0)
 * Range:     u8 n <= 9219, u16 n <= 9219, u32 n <= 9219, u64 n <= 9219,
 *            u128 n <= 9219, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* local struct for sorting keys (length + value) */
typedef struct {
    unsigned len;   /* length of the substring, 1..64 */
    uint64_t val;   /* integer value of the bits  */
} SubStrKey;

/* comparison function for qsort */
static int cmp_key(const void *a, const void *b)
{
    const SubStrKey *A = (const SubStrKey *)a;
    const SubStrKey *B = (const SubStrKey *)b;
    if (A->len != B->len)
        return (A->len < B->len) ? -1 : 1;
    if (A->val != B->val)
        return (A->val < B->val) ? -1 : 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the sequence, safe guard */

    /* compute bit length */
    unsigned len = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        len++;
    }

    /* bits[0] = most significant bit */
    unsigned bits[64];
    for (unsigned i = 0; i < len; ++i) {
        bits[i] = (n >> (len - 1 - i)) & 1U;
    }

    /* maximum number of substrings */
    const unsigned max_sub = len * (len + 1) / 2;
    SubStrKey keys[2080];   /* 2080 is enough for len==64 */
    unsigned count = 0;

    /* generate all substrings */
    for (unsigned start = 0; start < len; ++start) {
        uint64_t val = 0;
        for (unsigned l = 1; start + l <= len; ++l) {
            val = (val << 1) | bits[start + l - 1];
            keys[count].len = l;
            keys[count].val = val;
            ++count;
        }
    }

    /* sort and count distinct */
    qsort(keys, count, sizeof(SubStrKey), cmp_key);
    unsigned distinct = 0;
    if (count) {
        ++distinct;                /* first element distinct */
        for (unsigned i = 1; i < count; ++i) {
            if (keys[i].len != keys[i - 1].len || keys[i].val != keys[i - 1].val)
                ++distinct;
        }
    }

    return (seq_t)distinct;
}
