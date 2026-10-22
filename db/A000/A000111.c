/*
 * A000111  Euler or up/down numbers: e.g.f. sec(x) + tan(x). Also for n >= 2,
 *          half the number of alternating permutations on n letters
 *          (A001250).
 * https://oeis.org/A000111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, eigen, nice, easy
 * Formula:   Seidel's boustrophedon (Entringer triangle): E(0,0) = 1, E(k,0)
 *            = 0 for k >= 1, E(k,j) = E(k,j-1) + E(k-1,k-j) for 1 <= j <= k;
 *            a(n) = E(n,n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A111_MAXN 20005

static seq_t a111_buf0[A111_MAXN];
static seq_t a111_buf1[A111_MAXN];
static seq_t a111_cache[A111_MAXN];
static seq_t *a111_prev = a111_buf0;
static seq_t *a111_cur = a111_buf1;
static size_t a111_done = 0;
static int a111_init = 0;

seq_t a(idx_t n)
{
    size_t k, j;
    if (!a111_init) {
        a111_buf0[0] = 1;
        a111_cache[0] = 1;
        a111_done = 0;
        a111_init = 1;
    }
    if (n >= (idx_t)A111_MAXN)
        return 0;
    while (a111_done < (size_t)n) {
        seq_t *t;
        k = a111_done + 1;
        a111_cur[0] = 0;
        for (j = 1; j <= k; j++) {
            a111_cur[j] = a111_cur[j - 1];
            a111_cur[j] += a111_prev[k - j];
        }
        a111_cache[k] = a111_cur[k];
        t = a111_prev;
        a111_prev = a111_cur;
        a111_cur = t;
        a111_done = k;
    }
    return a111_cache[(size_t)n];
}
