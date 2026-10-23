/*
 * A025404  Numbers that are the sum of 4 positive cubes in exactly 2 ways.
 * https://oeis.org/A025404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025404 – Numbers that are the sum of four positive cubes in exactly two unordered ways.
   The routine generates all such numbers up to an adaptive upper bound, collects them in
   ascending order, and returns the n‑th term (1‑based indexing).   */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *seq_values = NULL;   /* table of found terms   */
static idx_t  seq_len   = 0;       /* number of terms stored */
static idx_t  seq_cap   = 0;       /* allocated capacity    */
static int    initialized = 0;     /* flag indicating that the table has been built */

/* enlarge the table if needed */
static void grow_seq(idx_t new_cap)
{
    if (new_cap <= seq_cap) return;
    seq_t *p = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    seq_values = p;
    seq_cap = new_cap;
}

/* integer cube root floor(n^(1/3)) using binary search on idx_t */
static size_t int_cube_root(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * hi * hi <= n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * mid * mid <= n) lo = mid;
        else hi = mid;
    }
    return (size_t)lo;
}

/* build the table of terms up to enough entries for typical queries */
static void build_sequence(void)
{
    const idx_t TARGET_TERMS = 60000;   /* how many terms we will store */
    idx_t Nmax = 1000000;              /* initial upper bound */

    while (1) {
        size_t root = int_cube_root(Nmax);

        /* array counting how many ways each number can be written as a sum of 4 cubes */
        uint16_t *cnt = (uint16_t *)calloc((size_t)Nmax + 1, sizeof(uint16_t));
        if (!cnt) exit(1);

        /* pre‑compute cubes 1..root */
        seq_t *cube = (seq_t *)malloc((root + 1) * sizeof(seq_t));
        if (!cube) { free(cnt); exit(1); }
        for (size_t i = 1; i <= root; ++i) cube[i] = (seq_t)i * (seq_t)i * (seq_t)i;

        /* enumerate unordered quadruples a<=b<=c<=d */
        for (size_t a = 1; a <= root; ++a) {
            seq_t ca = cube[a];
            for (size_t b = a; b <= root; ++b) {
                seq_t sum2 = ca + cube[b];
                if (sum2 > Nmax) break;
                for (size_t c = b; c <= root; ++c) {
                    seq_t sum3 = sum2 + cube[c];
                    if (sum3 > Nmax) break;
                    for (size_t d = c; d <= root; ++d) {
                        seq_t sum4 = sum3 + cube[d];
                        if (sum4 > Nmax) break;
                        ++cnt[(size_t)sum4];
                    }
                }
            }
        }
        free(cube);

        /* store numbers that have exactly two representations */
        for (idx_t val = 1; val <= Nmax; ++val) {
            if (cnt[(size_t)val] == 2) {
                if (seq_len == seq_cap) grow_seq(seq_len + 1024);
                seq_values[seq_len++] = (seq_t)val;
            }
        }
        free(cnt);

        if (seq_len >= TARGET_TERMS) break;   /* sufficient terms collected */

        Nmax <<= 1;   /* if not enough, double the search bound */
    }

    initialized = 1;
}

/* required public function */
seq_t a(idx_t n)
{
    if (!initialized) build_sequence();
    if (n == 0 || n > seq_len) return 0;   /* guard against invalid indices */
    return seq_values[n - 1];
}
