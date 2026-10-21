/*
 * A079339  Least k such that the decimal representation of k*n contains only
 *          1's and 0's.
 * https://oeis.org/A079339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the least k such that k*n has only decimal digits 0 or 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 35, u64 n <= 998,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, but guard against 0 */

    /* Use the remainder modulo n to perform a BFS that finds the
       smallest multiple of n whose decimal digits are only 0 and 1. */
    size_t mod = (size_t)n;

    /* Arrays for BFS: parent remainder, the digit appended to reach it,
       and a visited flag. */
    int  *parent = (int *)malloc(mod * sizeof(int));
    char *digit  = (char *)malloc(mod * sizeof(char));
    bool *vis    = (bool *)malloc(mod * sizeof(bool));
    if (!parent || !digit || !vis) {
        free(parent); free(digit); free(vis);
        return 0;
    }
    for (size_t i = 0; i < mod; ++i) vis[i] = false;

    /* Queue for BFS over remainders. */
    int *queue = (int *)malloc((mod + 1) * sizeof(int));
    if (!queue) {
        free(parent); free(digit); free(vis);
        return 0;
    }
    size_t head = 0, tail = 0;

    /* Start from the number "1". */
    size_t start = 1 % mod;
    parent[start] = -1;
    digit[start]  = '1';
    vis[start]    = true;
    queue[tail++] = (int)start;

    /* Special case when n divides 1. */
    if (start == 0) {
        free(parent); free(digit); free(vis); free(queue);
        seq_t k = seq_from_str("1");
        return k / n;
    }

    /* BFS loop – explore remainders by appending 0 or 1. */
    while (head < tail) {
        size_t r = (size_t)queue[head++];
        /* Append digit 0: new remainder = r*10 (mod n). */
        size_t r0 = (r * 10ULL) % mod;
        if (!vis[r0]) {
            vis[r0]    = true;
            parent[r0] = (int)r;
            digit[r0]  = '0';
            if (r0 == 0) break;
            queue[tail++] = (int)r0;
        }
        /* Append digit 1: new remainder = r*10 + 1 (mod n). */
        size_t r1 = ((r * 10ULL) + 1ULL) % mod;
        if (!vis[r1]) {
            vis[r1]    = true;
            parent[r1] = (int)r;
            digit[r1]  = '1';
            if (r1 == 0) break;
            queue[tail++] = (int)r1;
        }
    }

    /* Reconstruct the smallest non‑negative integer m whose remainder
       is 0, by following parent pointers from remainder 0 back to the
       start. The digits are collected in reverse order. */
    size_t max_len = mod + 1;
    char *rev = (char *)malloc(max_len * sizeof(char));
    if (!rev) {
        free(parent); free(digit); free(vis); free(queue);
        return 0;
    }
    size_t pos = 0;
    int cur = (int)0;                /* remainder 0 */
    while (cur != -1) {
        rev[pos++] = digit[cur];
        cur = parent[cur];
    }

    /* Reverse to obtain the digits in the correct order. */
    char *num = (char *)malloc((pos + 1) * sizeof(char));
    if (!num) {
        free(parent); free(digit); free(vis); free(queue); free(rev);
        return 0;
    }
    for (size_t i = 0; i < pos; ++i) num[i] = rev[pos - 1 - i];
    num[pos] = '\0';

    /* Convert the string of digits to the seq_t multiple m. */
    seq_t m = 0;
    for (size_t i = 0; i < pos; ++i) {
        m = m * (seq_t)10 + (seq_t)(num[i] - '0');
    }

    /* The desired k is m/n. */
    seq_t k = m / (seq_t)n;

    /* Clean up allocated memory. */
    free(parent); free(digit); free(vis); free(queue); free(rev); free(num);

    return k;
}
