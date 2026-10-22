/*
 * A004826  Numbers that are the sum of at most 4 positive cubes.
 * https://oeis.org/A004826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve: mark every number up to a limit that is a sum of at most 4 positive cubes (0 counts as the empty sum), list the marked numbers in increasing order, and double the limit until there are at least n of them. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a4826_vals = NULL;
static uint64_t a4826_count = 0;

static void a4826_build(uint64_t L) {
    unsigned char *s1, *s2, *s3, *s4;
    uint64_t *cubes;
    uint64_t nc = 0, c, v, i, k;
    s1 = (unsigned char *)calloc(L + 1, 1);
    s2 = (unsigned char *)calloc(L + 1, 1);
    s3 = (unsigned char *)calloc(L + 1, 1);
    s4 = (unsigned char *)calloc(L + 1, 1);
    cubes = (uint64_t *)malloc(4096 * sizeof(uint64_t));
    s1[0] = 1;
    for (c = 1; c * c * c <= L; c++) {
        cubes[nc++] = c * c * c;
        s1[c * c * c] = 1;
    }
    /* s(k+1) = s(k) + {0, cubes} */
    {
        unsigned char *src[3];
        unsigned char *dst[3];
        src[0] = s1; dst[0] = s2;
        src[1] = s2; dst[1] = s3;
        src[2] = s3; dst[2] = s4;
        for (k = 0; k < 3; k++) {
            for (v = 0; v <= L; v++) {
                if (!src[k][v]) continue;
                dst[k][v] = 1;
                for (i = 0; i < nc && v + cubes[i] <= L; i++)
                    dst[k][v + cubes[i]] = 1;
            }
        }
    }
    free(a4826_vals);
    a4826_count = 0;
    for (v = 0; v <= L; v++) if (s4[v]) a4826_count++;
    a4826_vals = (uint64_t *)malloc((a4826_count + 1) * sizeof(uint64_t));
    a4826_count = 0;
    for (v = 0; v <= L; v++) if (s4[v]) a4826_vals[a4826_count++] = v;
    free(s1); free(s2); free(s3); free(s4); free(cubes);
}

seq_t a(idx_t n) {
    uint64_t L = 1024;
    while (a4826_count < n) {
        a4826_build(L);
        L *= 2;
    }
    return (seq_t)a4826_vals[n - 1];
}
