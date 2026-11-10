/*
 * A086987  a(n) = A000203(A086986(n)).
 * https://oeis.org/A086987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A086986(n)); A086986 found by a sieve: j is a term
 *            iff sigma(j) is a sum of >=2 consecutive sigma values
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9921 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t A000203_a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            result *= term;
        }
    }
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }
    return result;
}

static uint64_t *A086987_terms = NULL;
static size_t A086987_count = 0;

static void A086987_build(uint64_t J)
{
    uint64_t L = 3 * J + 10;
    uint64_t i, j, m, e;
    uint64_t *sg = (uint64_t *)malloc((L + 2) * sizeof(uint64_t));
    uint64_t *pre = (uint64_t *)malloc((L + 2) * sizeof(uint64_t));
    uint64_t maxS = 0;
    uint8_t *flag;
    uint64_t *terms;
    size_t cnt = 0;
    if (!sg || !pre) exit(1);
    memset(sg, 0, (L + 2) * sizeof(uint64_t));
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i) sg[j] += i;
    for (i = 1; i <= J; i++)
        if (sg[i] > maxS) maxS = sg[i];
    pre[0] = 0;
    for (i = 1; i <= L; i++) pre[i] = pre[i - 1] + sg[i];
    flag = (uint8_t *)calloc(maxS + 1, 1);
    if (!flag) exit(1);
    for (m = 1; m < L; m++) {
        if (sg[m] + sg[m + 1] > maxS) {
            /* sigma(m) >= m+1, so later starts only grow past maxS */
            if (m > maxS) break;
        }
        for (e = m + 1; e <= L; e++) {
            uint64_t s = pre[e] - pre[m - 1];
            if (s > maxS) break;
            flag[s] = 1;
        }
    }
    terms = (uint64_t *)malloc((J + 1) * sizeof(uint64_t));
    if (!terms) exit(1);
    for (j = 1; j <= J; j++)
        if (flag[sg[j]]) terms[cnt++] = j;
    free(A086987_terms);
    A086987_terms = terms;
    A086987_count = cnt;
    free(sg);
    free(pre);
    free(flag);
}

seq_t a(idx_t n)
{
    static uint64_t J = 0;
    if (J == 0) {
        J = 200000;
        A086987_build(J);
    }
    while ((uint64_t)A086987_count < (uint64_t)n) {
        J *= 2;
        A086987_build(J);
    }
    return A000203_a((idx_t)A086987_terms[(size_t)(n - 1)]);
}
