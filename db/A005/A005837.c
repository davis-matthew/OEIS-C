/*
 * A005837  Lexicographically earliest increasing sequence of positive numbers
 *          that contains no 4-term arithmetic progression.
 * https://oeis.org/A005837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 4077, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy search: a(n) is the least value > a(n-1) that does not complete a 4-term AP
   (x-3d, x-2d, x-d, x) with earlier terms; a(1) = 1.
   When a term c is added, every d with c-d and c-2d in the sequence forbids c+d. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static long long *vals = NULL;
static idx_t cnt = 0, vcap = 0;
static unsigned char *inset = NULL;
static unsigned char *forb = NULL;
static size_t bsz = 0;

static void ensure_bits(size_t need) {
    if (need <= bsz) return;
    size_t nsz = bsz ? bsz : 1024;
    while (nsz < need) nsz *= 2;
    inset = (unsigned char *)realloc(inset, nsz);
    forb = (unsigned char *)realloc(forb, nsz);
    memset(inset + bsz, 0, nsz - bsz);
    memset(forb + bsz, 0, nsz - bsz);
    bsz = nsz;
}

static void ensure_vals(idx_t need) {
    if (need <= vcap) return;
    idx_t ncap = vcap ? vcap : 256;
    while (ncap < need) ncap *= 2;
    vals = (long long *)realloc(vals, ncap * sizeof(long long));
    vcap = ncap;
}

seq_t a(idx_t n) {
    if (n > cnt) {
        ensure_vals(n);
        long long x = cnt ? vals[cnt - 1] + 1 : 1;
        while (cnt < n) {
            ensure_bits((size_t)(2 * x + 2));
            if (!forb[x]) {
                for (idx_t i = cnt; i > 0; i--) {
                    long long y = vals[i - 1];
                    long long d = x - y;
                    if (y - d < 1) break;
                    if (inset[y - d]) forb[x + d] = 1;
                }
                inset[x] = 1;
                vals[cnt++] = x;
            }
            x++;
        }
    }
    return (seq_t)vals[n - 1];
}
