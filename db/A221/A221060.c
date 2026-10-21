/*
 * A221060  Equals one maps: number of nX4 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal and antidiagonal neighbors in a random 0..3 nX4 array.
 * https://oeis.org/A221060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 38*a(n-1) - 527*a(n-2) + 3412*a(n-3) - 10701*a(n-4) + 15146*a(n-5)
 *              - 10333*a(n-6) + 12220*a(n-7) - 16455*a(n-8) + 6382*a(n-9) - 2097*a(n-10)
 *              + 1296*a(n-11)   for n>14
 * Initial values:
 * a(1)=8, a(2)=172, a(3)=3322, a(4)=58710, a(5)=989020, a(6)=16261308,
 * a(7)=263988148, a(8)=4256750870, a(9)=68391745316, a(10)=1096704806282,
 * a(11)=17568167732952, a(12)=281269561878252, a(13)=4501843454461912,
 * a(14)=72042583248803298, a(15)=1152793219615408052, a(16)=18445647830197379358
 *
 * Computes A221060: the number of n×4 binary arrays indicating the locations
 * of elements equal to exactly one of their horizontal and antidiagonal neighbors
 * in a random 0..3 n×4 array.
 *
 * This implementation uses a static cache of size 10001 (enough for n up to ~10k).
 * It supports any seq_t (unsigned integer or big‑integer class) due to
 * implicit conversions in the compiler.  No dynamic allocation is required
 * beyond the static array, making it safe in both C11 and C++17.
 */

#include <stddef.h>

static const seq_t base_vals[17] = {
    0ULL,
    8ULL,
    172ULL,
    3322ULL,
    58710ULL,
    989020ULL,
    16261308ULL,
    263988148ULL,
    4256750870ULL,
    68391745316ULL,
    1096704806282ULL,
    17568167732952ULL,
    281269561878252ULL,
    4501843454461912ULL,
    72042583248803298ULL,
    1152793219615408052ULL,
    18445647830197379358ULL
};

static seq_t cache[10001];        /* static cache, 1‑based indexing */
static idx_t cached_max = 0;      /* largest n already computed */

/* Compute A221060 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0; return 0 for safety */

    /* Ensure base values up to 16 are cached */
    if (cached_max < 16) {
        size_t init_n = (n < 16) ? n : 16;
        for (idx_t i = cached_max + 1; i <= init_n; ++i)
            cache[i] = base_vals[i];
        if (init_n > cached_max)
            cached_max = init_n;
    }

    /* Compute additional terms up to n if needed */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i] = base_vals[i];
            continue;
        }
        seq_t val = (seq_t)38   * cache[i-1];
        val -= (seq_t)527   * cache[i-2];
        val += (seq_t)3412  * cache[i-3];
        val -= (seq_t)10701 * cache[i-4];
        val += (seq_t)15146 * cache[i-5];
        val -= (seq_t)10333 * cache[i-6];
        val += (seq_t)12220 * cache[i-7];
        val -= (seq_t)16455 * cache[i-8];
        val += (seq_t)6382  * cache[i-9];
        val -= (seq_t)2097  * cache[i-10];
        val += (seq_t)1296  * cache[i-11];
        cache[i] = val;
    }
    if (n > cached_max)
        cached_max = n;

    return cache[n];
}
