/*
 * A057509  Permutation of natural numbers: rotations of the bottom branches
 *          of the rooted plane trees encoded by A014486.
 * https://oeis.org/A057509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index in A014486 of the plane tree obtained from tree
 *            A014486(n) by moving its first root branch to the last position
 *            (rotation of bottom branches); a(0)=0, a(1)=1, a(2)=2, a(3)=3.
 *            Computed by unranking/ranking Dyck words
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A057509_M 82

static uint64_t A057509_W[A057509_M][A057509_M];
static uint64_t A057509_cat[42];
static bool A057509_ready = false;

static void A057509_init(void)
{
    int r, h;
    if (A057509_ready) return;
    for (r = 0; r < A057509_M; r++)
        for (h = 0; h < A057509_M; h++)
            A057509_W[r][h] = 0;
    A057509_W[0][0] = 1;
    for (r = 1; r < A057509_M; r++) {
        for (h = 0; h < A057509_M - 1; h++) {
            uint64_t v = A057509_W[r - 1][h + 1];
            if (h > 0) v += A057509_W[r - 1][h - 1];
            A057509_W[r][h] = v;
        }
    }
    for (r = 0; r < 42; r++)
        A057509_cat[r] = A057509_W[2 * r][0];
    A057509_ready = true;
}

seq_t a(idx_t n)
{
    int bits[90], nb[90];
    int k = 0, L, i, h, rem;
    uint64_t r = (uint64_t)n;
    uint64_t base = 0, rank = 0;

    A057509_init();

    /* find size k and rank r within size */
    while (k < 40 && r >= A057509_cat[k]) {
        r -= A057509_cat[k];
        base += A057509_cat[k];
        k++;
    }
    if (k == 0) return (seq_t)0;
    L = 2 * k;

    /* unrank: 0 (down) sorts before 1 (up) */
    h = 0;
    for (i = 0; i < L; i++) {
        rem = L - i;
        uint64_t c = 0;
        if (h > 0) c = A057509_W[rem - 1][h - 1];
        if (h > 0 && r < c) {
            bits[i] = 0;
            h--;
        } else {
            r -= c;
            bits[i] = 1;
            h++;
        }
    }

    /* first top-level branch ends where height first returns to 0 */
    h = 0;
    int e = 0;
    for (i = 0; i < L; i++) {
        h += bits[i] ? 1 : -1;
        if (h == 0) { e = i + 1; break; }
    }
    /* rotate: move first branch to the end */
    {
        int p = 0;
        for (i = e; i < L; i++) nb[p++] = bits[i];
        for (i = 0; i < e; i++) nb[p++] = bits[i];
    }

    /* rank */
    h = 0;
    for (i = 0; i < L; i++) {
        rem = L - i;
        if (nb[i] == 1) {
            if (h > 0) rank += A057509_W[rem - 1][h - 1];
            h++;
        } else {
            h--;
        }
    }
    return (seq_t)(base + rank);
}
