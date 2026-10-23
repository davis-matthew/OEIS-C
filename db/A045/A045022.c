/*
 * A045022  Numbers whose base-4 representation contains exactly one 0 and
 *          four 1's.
 * https://oeis.org/A045022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 221, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045022: numbers whose base‑4 representation contains exactly
   one 0 and four 1's.  The function generates all such numbers with
   length up to 9 (enough for the first 10 000 terms), sorts them, and
   then returns the n‑th term.  */
#include <stdlib.h>

static seq_t seq_arr[12000];
static size_t seq_len = 0;
static int seq_inited = 0;

static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Generate numbers of length L and append them to seq_arr. */
static void gen_length(unsigned L)
{
    unsigned zeroPos;
    for (zeroPos = 1; zeroPos < L; ++zeroPos) {
        /* positions available for the four 1's (exclude zeroPos) */
        unsigned avail[12], availCnt = 0;
        unsigned pos;
        for (pos = 0; pos < L; ++pos)
            if (pos != zeroPos) avail[availCnt++] = pos;

        /* enumerate all 4‑tuples of positions for the 1's */
        int a, b, c, d;
        for (a = 0; a <= (int)availCnt - 4; ++a)
        for (b = a + 1; b <= (int)availCnt - 3; ++b)
        for (c = b + 1; c <= (int)availCnt - 2; ++c)
        for (d = c + 1; d <= (int)availCnt - 1; ++d) {
            unsigned ones[4] = { avail[a], avail[b], avail[c], avail[d] };

            /* remaining positions to be 2 or 3 */
            unsigned rem[12], remCnt = 0;
            for (pos = 0; pos < L; ++pos) {
                if (pos == zeroPos) continue;
                if (pos == ones[0] || pos == ones[1] ||
                    pos == ones[2] || pos == ones[3]) continue;
                rem[remCnt++] = pos;
            }

            unsigned maskMax = 1u << remCnt;
            unsigned mask;
            for (mask = 0; mask < maskMax; ++mask) {
                unsigned digits[12];
                unsigned i;
                for (i = 0; i < L; ++i) digits[i] = 2;
                digits[zeroPos] = 0;
                digits[ones[0]] = digits[ones[1]] =
                digits[ones[2]] = digits[ones[3]] = 1;
                for (i = 0; i < remCnt; ++i)
                    digits[rem[i]] = (mask >> i) & 1 ? 3 : 2;

                seq_t val = 0;
                for (i = 0; i < L; ++i)
                    val = val * 4 + (seq_t)digits[i];

                seq_arr[seq_len++] = val;
            }
        }
    }
}

static void init_sequence(void)
{
    unsigned L;
    for (L = 5; L <= 9; ++L)
        gen_length(L);
    qsort(seq_arr, seq_len, sizeof(seq_t), cmp_seq);
    seq_inited = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!seq_inited) init_sequence();
    if ((size_t)n > seq_len) return (seq_t)0;   /* out of range */
    return seq_arr[(size_t)n - 1];
}
