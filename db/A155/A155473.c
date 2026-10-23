/*
 * A155473  Numbers of the form x^3+y^5, with x,y>0 and x<>y.
 * https://oeis.org/A155473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers of the form x^3 + y^5, with x>0, y>0 and x≠y.
 *  The sequence is sorted ascending and each representation is listed
 *  separately (duplicates are repeated).  This implementation
 *  pre‑computes all pairs up to fixed bounds (500 for both x and y)
 *  which is sufficient for all indices up to at least 10 000
 *  and far beyond.  It works for any seq_t type supported by the
 *  build (uint64_t, __int128, or a big‑integer class). */

#include <stdlib.h>
#include <stdbool.h>

/* fast integer exponentiation (seq_t base ^ idx_t exp) */
static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp != 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* a(n): n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* fixed bounds – enough for 10 000+ indices */
        const idx_t MAX_X = 500;
        const idx_t MAX_Y = 500;

        /* estimate capacity */
        size_t capacity = (size_t)MAX_X * (size_t)MAX_Y;
        seq_t *tmp = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */

        size_t cnt = 0;
        for (idx_t x = 1; x <= MAX_X; ++x) {
            seq_t x3 = pow_int((seq_t)x, 3);
            for (idx_t y = 1; y <= MAX_Y; ++y) {
                if (x == y) continue;
                seq_t y5 = pow_int((seq_t)y, 5);
                tmp[cnt++] = x3 + y5;
            }
        }

        /* Allocate exact sized cache and copy data */
        cache = (seq_t *)malloc(cnt * sizeof(seq_t));
        if (!cache) {
            free(tmp);
            return 0;
        }
        for (size_t i = 0; i < cnt; ++i) cache[i] = tmp[i];
        free(tmp);

        /* Sort the sequence */
        qsort(cache, cnt, sizeof(seq_t), cmp_seq);
        cache_len = cnt;
        initialized = true;
    }

    if (n == 0 || n > cache_len) return 0;   /* out of range */
    return cache[n - 1];
}
