/*
 * A180971  Numbers k such that 6^7 + k^2 is a square.
 * https://oeis.org/A180971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: k is the difference of complementary divisors of 69984
   (since 6^7 + k^2 = (m)^2  ⇔  (m-k)(m+k)=6^7 and both factors
   must be even, let a'= (m-k)/2, b'= (m+k)/2, then a'·b' = 69984.
   Hence all k = b' - a' for all divisor pairs (a',b') with a' < b'.
   The sequence is the kth smallest such k. */

#include <stdbool.h>

static seq_t  _k_cache[64];   /* enough to hold all 24 values */
static idx_t  _k_count = 0;   /* number of computed terms        */
static bool   _k_initialized = false;

static void
_k_init_once(void)
{
    if (_k_initialized) return;
    _k_initialized = true;

    const idx_t N = 69984ULL;          /* 6^7 / 4 */

    /* compute floor(sqrt(N)) without using helper functions */
    idx_t sqrtN = 0;
    for (idx_t x = 1; ; ++x) {
        if (x > N / x) break;   /* avoid overflow */
        sqrtN = x;
    }

    /* gather all differences k = b' - a' for a' ≤ b' */
    int tmp[64];
    int tmp_cnt = 0;

    for (idx_t a = 1; a <= sqrtN; ++a) {
        if (N % a == 0) {
            idx_t b = N / a;
            seq_t k = (seq_t)(b - a);   /* k fits in int (≤ 69983) */
            tmp[tmp_cnt++] = (int)k;
        }
    }

    /* sort the small array */
    for (int i = 1; i < tmp_cnt; ++i) {
        int key = tmp[i];
        int j = i - 1;
        while (j >= 0 && tmp[j] > key) {
            tmp[j + 1] = tmp[j];
            --j;
        }
        tmp[j + 1] = key;
    }

    _k_count = (idx_t)tmp_cnt;
    for (int i = 0; i < tmp_cnt; ++i) {
        _k_cache[i] = (seq_t)tmp[i];
    }
}

seq_t a(idx_t n)
{
    _k_init_once();
    if (n == 0 || n > _k_count) return 0;
    return _k_cache[n - 1];
}
