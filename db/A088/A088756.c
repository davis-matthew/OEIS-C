/*
 * A088756  a(n) = A088599(n)/n.
 * https://oeis.org/A088756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A088599(n)/n, where A088599(n) is the smallest multiple
 *            of n with exactly one nonzero digit (found by BFS over
 *            remainders mod n for each digit d=1..9)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t A088756_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v * 10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static void A088756_build(idx_t n, int d, char **out, size_t *olen)
{
    int N = (int)n;
    int *parent = (int*)malloc((size_t)N * sizeof(int));
    uint8_t *dig = (uint8_t*)malloc((size_t)N * sizeof(uint8_t));
    int *queue = (int*)malloc((size_t)N * sizeof(int));
    if (!parent || !dig || !queue) abort();

    for (int i = 0; i < N; ++i) parent[i] = -2;

    int front = 0, back = 0;
    int r0 = d % N;
    parent[r0] = -1;
    dig[r0] = (uint8_t)d;
    queue[back++] = r0;

    while (front < back) {
        int rem = queue[front++];
        if (rem == 0) break;
        for (int k = 0; k < 2; ++k) {
            int add = (k == 0) ? 0 : d;
            int newrem = (int)(((unsigned long long)rem * 10ULL + (unsigned)add) % (unsigned)N);
            if (parent[newrem] == -2) {
                parent[newrem] = rem;
                dig[newrem] = (uint8_t)add;
                queue[back++] = newrem;
                if (newrem == 0) break;
            }
        }
    }

    size_t len = 0;
    char *buf = NULL;
    int cur = 0;
    while (cur != -1) {
        ++len;
        buf = (char*)realloc(buf, len);
        buf[len - 1] = (char)('0' + dig[cur]);
        cur = parent[cur];
    }
    for (size_t i = 0; i < len / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
    buf = (char*)realloc(buf, len + 1);
    buf[len] = '\0';

    *out = buf;
    *olen = len;
    free(parent);
    free(dig);
    free(queue);
}

static int A088756_cmp(const char *a, size_t alen, const char *b, size_t blen)
{
    if (alen < blen) return -1;
    if (alen > blen) return 1;
    int r = memcmp(a, b, alen);
    return (r < 0) ? -1 : (r > 0) ? 1 : 0;
}

static seq_t A088599_a(idx_t n)
{
    if (n == 0) return 0;
    char *best = NULL;
    size_t bestlen = 0;
    bool hasbest = false;

    for (int d = 1; d <= 9; ++d) {
        char *cand = NULL;
        size_t candlen = 0;
        A088756_build(n, d, &cand, &candlen);
        int cmp = hasbest ? A088756_cmp(cand, candlen, best, bestlen) : -1;
        if (cmp < 0) {
            if (hasbest) free(best);
            best = cand;
            bestlen = candlen;
            hasbest = true;
        } else {
            free(cand);
        }
    }
    seq_t value = A088756_from_str(best);
    free(best);
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A088599_a(n) / (seq_t)n;
}
