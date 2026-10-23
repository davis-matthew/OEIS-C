/*
 * A045187  Numbers whose base-5 representation contains exactly three 0's and
 *          three 2's.
 * https://oeis.org/A045187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑5 representation
 *            contains exactly three 0's and exactly three 2's. Sequence
 *            A045187
 * Range:     u8 none, u16 n <= 240, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache      = NULL; /* stored terms */
static size_t  cap       = 0;     /* current capacity of cache   */
static size_t  cnt       = 0;     /* number of terms stored       */
static idx_t   cur_len   = 6;     /* current length being processed */
static bool    init_done = false;

/* Recursive generator for a fixed length L */
static void gen_rec(int pos, int len, seq_t val, int zeros, int twos)
{
    if (pos == len) {
        if (zeros == 3 && twos == 3) {
            if (cnt == cap) {
                cap = (cap == 0) ? 64 : cap * 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cnt++] = val;
        }
        return;
    }

    /* Prune impossible branches */
    if (zeros > 3 || twos > 3) return;
    int rem = len - pos;
    int needZeros = 3 - zeros;
    int needTwos  = 3 - twos;
    if (needZeros + needTwos > rem) return;
    if (zeros + rem < 3 || twos + rem < 3) return;

    for (int d = 0; d <= 4; ++d) {
        if (pos == 0 && d == 0) continue;          /* leading digit ≠ 0 */
        int nz = zeros, nt = twos;
        if (d == 0) ++nz;
        else if (d == 2) ++nt;
        /* val * 5 + d works for seq_t (includes big integers) */
        gen_rec(pos + 1, len, val * 5 + (seq_t)d, nz, nt);
    }
}

seq_t a(idx_t n)
{
    if (!init_done) {
        /* initialisation: allocate cache */
        cap  = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        init_done = true;
    }

    /* generate enough terms */
    while (cnt < n) {
        gen_rec(0, (int)cur_len, (seq_t)0, 0, 0);
        ++cur_len;
    }

    return cache[n - 1];
}
