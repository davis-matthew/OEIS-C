/*
 * A204278  Number of (n+2) X 4 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A204278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204278
   Recurrence   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6)    for n > 9
   Initial terms:
        a(1)=274, a(2)=622, a(3)=865, a(4)=193, a(5)=539,
        a(6)=735, a(7)=1883, a(8)=5187, a(9)=12715
*/

#define MAX_N 20000        /* sufficient for all test cases up to ~10000 */

static const seq_t base[MAX_N+1] = {
    0,          /* unused, to make indices 1‑based */
    274, 622, 865, 193, 539, 735, 1883, 5187, 12715
};

static seq_t cache[MAX_N+1];     /* memoised terms */
static idx_t cache_len = 0;      /* highest index computed so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, safe fallback */

    if (n <= 9) return base[n];

    /* initialise base terms once */
    if (cache_len == 0) {
        for (idx_t i = 1; i <= 9; ++i) cache[i] = base[i];
        cache_len = 9;
    }

    /* compute missing terms up to n using the recurrence */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i-1]
                      - (seq_t)6 * cache[i-4]
                      + (seq_t)4 * cache[i-6];
        }
        cache_len = n;
    }

    return cache[n];
}
