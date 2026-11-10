/*
 * A206473  First prime in A122072 that appears at least n times.
 * https://oeis.org/A206473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p in A122072 with at least n occurrences,
 *            i.e. floor(q/10) - floor(p/10) >= n where q = nextprime(p);
 *            computed by an incremental segmented sieve
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A206473_MAXN 128
#define A206473_BASE_LIM 1100000u
#define A206473_SEG 262144u /* odd numbers per segment */

static uint64_t A206473_first[A206473_MAXN + 1];
static bool A206473_init = false;
static uint32_t *A206473_bp = NULL;
static size_t A206473_bpn = 0;
static uint64_t A206473_lo = 0;   /* next segment start (even) */
static uint64_t A206473_last = 2; /* last prime seen */
static unsigned char *A206473_buf = NULL;

static void A206473_setup(void)
{
    uint32_t L = A206473_BASE_LIM;
    unsigned char *s = (unsigned char *)calloc(L + 1, 1);
    A206473_bp = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    A206473_bpn = 0;
    for (uint32_t i = 2; i <= L; i++) {
        if (!s[i]) {
            if (i > 2) A206473_bp[A206473_bpn++] = i;
            if ((uint64_t)i * i <= L)
                for (uint32_t m = i * i; m <= L; m += i) s[m] = 1;
        }
    }
    free(s);
    A206473_buf = (unsigned char *)malloc(A206473_SEG);
    memset(A206473_first, 0, sizeof(A206473_first));
    A206473_init = true;
}

static void A206473_segment(void)
{
    uint64_t lo = A206473_lo;
    uint64_t hi = lo + 2ull * A206473_SEG;
    memset(A206473_buf, 1, A206473_SEG);
    if (lo == 0) A206473_buf[0] = 0; /* number 1 */
    for (size_t k = 0; k < A206473_bpn; k++) {
        uint64_t p = A206473_bp[k];
        if (p * p >= hi) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        if (m < p * p) m = p * p;
        if ((m & 1) == 0) m += p;
        for (uint64_t i = (m - lo - 1) / 2; i < A206473_SEG; i += p)
            A206473_buf[i] = 0;
    }
    for (uint64_t i = 0; i < A206473_SEG; i++) {
        if (!A206473_buf[i]) continue;
        uint64_t q = lo + 2 * i + 1;
        uint64_t c = q / 10 - A206473_last / 10;
        if (c > 0) {
            if (c > A206473_MAXN) c = A206473_MAXN;
            for (uint64_t j = 1; j <= c; j++)
                if (!A206473_first[j]) A206473_first[j] = A206473_last;
        }
        A206473_last = q;
    }
    A206473_lo = hi;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > A206473_MAXN) return (seq_t)0;
    if (!A206473_init) A206473_setup();
    while (!A206473_first[n]) A206473_segment();
    return (seq_t)A206473_first[n];
}
