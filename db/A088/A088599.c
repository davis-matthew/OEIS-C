/*
 * A088599  Smallest multiple of n using only one nonzero digit.
 * https://oeis.org/A088599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 156, u64 n <= 6513,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9371 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Calculate the smallest positive multiple of n that uses exactly one
 *  non‑zero decimal digit (1–9) and any number of zeros.
 *
 *  For each digit d∈{1,…,9} we search the directed graph of remainders
 *  modulo n, where from a remainder r we can go to (r·10+0) mod n and
 *  (r·10+d) mod n.  A breadth‑first search finds the shortest string of
 *  digits {0,d} that ends in remainder 0 – i.e. the smallest multiple
 *  using only that digit.  The best result over all d is returned.
 *
 *  Complexity: O(9·n) time, O(n) memory per digit.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/*  Build the smallest string consisting of digits {0,d} that is a
 *  multiple of n.  `out` receives a malloc'ed NUL‑terminated string
 *  and its length (excluding the NUL) in `olen`.  Caller must free(*out). */
static void build_candidate(idx_t n, int d, char **out, size_t *olen)
{
    int N = (int)n;                          /* n≤10000 in tests */
    int *parent = (int*)malloc((size_t)N * sizeof(int));
    uint8_t *dig    = (uint8_t*)malloc((size_t)N * sizeof(uint8_t));
    int   *queue   = (int*)malloc((size_t)N * sizeof(int));
    if (!parent || !dig || !queue) abort();

    for (int i = 0; i < N; ++i) parent[i] = -2;   /* unvisited */

    int front = 0, back = 0;
    int r0 = d % N;
    parent[r0] = -1;                         /* start node */
    dig[r0]    = (uint8_t)d;
    queue[back++] = r0;

    /* BFS */
    while (front < back) {
        int rem = queue[front++];
        if (rem == 0) break;                  /* reached goal */

        /* try appending 0 and d (order matters for minimal numeric) */
        for (int k = 0; k < 2; ++k) {
            int add = (k == 0) ? 0 : d;
            int newrem = (int)(((unsigned long long)rem * 10ULL + (unsigned)add) % (unsigned)N);
            if (parent[newrem] == -2) {
                parent[newrem] = rem;
                dig[newrem]    = (uint8_t)add;
                queue[back++]  = newrem;
                if (newrem == 0) break;     /* goal reached */
            }
        }
    }

    /* Reconstruct the string (reverse order) */
    size_t len = 0;
    char *buf = NULL;
    int cur = 0;                               /* remainder 0 */
    while (cur != -1) {
        ++len;
        buf = (char*)realloc(buf, len);
        buf[len-1] = (char)('0' + dig[cur]);
        cur = parent[cur];
    }

    /* Reverse to correct order */
    for (size_t i = 0; i < len/2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[len-1-i];
        buf[len-1-i] = tmp;
    }

    /* NUL‑terminate */
    buf = (char*)realloc(buf, len + 1);
    buf[len] = '\0';

    *out  = buf;
    *olen = len;

    free(parent);
    free(dig);
    free(queue);
}

/*  Lexicographic comparison of two digit strings of known lengths.
 *  Returns -1 if a<b, 0 if equal, +1 if a>b.  Shorter string is
 *  always considered smaller (the numeric value is smaller). */
static int cmp_str(const char *a, size_t alen, const char *b, size_t blen)
{
    if (alen < blen) return -1;
    if (alen > blen) return 1;
    int r = memcmp(a, b, alen);
    return (r < 0) ? -1 : (r > 0) ? 1 : 0;
}

/*  Main sequence function requested by the build framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 → n≥1 expected */

    char *best = NULL;
    size_t bestlen = 0;
    bool hasbest = false;

    for (int d = 1; d <= 9; ++d) {
        char *cand = NULL;
        size_t candlen = 0;
        build_candidate(n, d, &cand, &candlen);

        int cmp = 0;
        if (!hasbest) cmp = -1;          /* any candidate beats none */
        else cmp = cmp_str(cand, candlen, best, bestlen);

        if (cmp < 0) {                  /* candidate is smaller */
            if (hasbest) free(best);
            best = cand;
            bestlen = candlen;
            hasbest = true;
        } else {
            free(cand);
        }
    }

    seq_t value = seq_from_str(best);
    free(best);
    return value;
}
