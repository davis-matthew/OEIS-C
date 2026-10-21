/*
 * A094209  Numbers n such that the sum of n's digits times the sum of the
 *          factorials of n's digits is equal to n.
 * https://oeis.org/A094209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate all digit multisets up to 9 digits, compute
   S = sum of digits, P = sum of factorials of digits,
   keep numbers where S*P equals the decimal representation
   having exactly the same digit multiset. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const seq_t fact[10] = {1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL,
                               720ULL, 5040ULL, 40320ULL, 362880ULL};

static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

static void dfs_generate(int digit, int remaining, int totalcnt,
                         int *cnt, seq_t S, seq_t P,
                         seq_t seq[], int *seq_len)
{
    if (digit == 10) {
        if (remaining != 0) return;
        if (totalcnt == 0) return;
        seq_t X = S * P;
        if (X == 0) return;

        int dcnt[10] = {0};
        int len = 0;
        seq_t tmp = X;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            dcnt[d]++;
            tmp /= 10;
            len++;
        }
        if (len != totalcnt) return;
        for (int i = 0; i < 10; ++i)
            if (dcnt[i] != cnt[i]) return;

        seq[*seq_len] = X;
        (*seq_len)++;
        return;
    }

    for (int k = 0; k <= remaining; ++k) {
        cnt[digit] = k;
        dfs_generate(digit + 1, remaining - k, totalcnt,
                     cnt, S + (seq_t)digit * k,
                     P + fact[digit] * (seq_t)k,
                     seq, seq_len);
    }
}

static void generate_all(seq_t seq[], int *seq_len)
{
    int cnt[10];
    for (int c = 1; c <= 9; ++c) {
        for (int i = 0; i < 10; ++i) cnt[i] = 0;
        dfs_generate(0, c, c, cnt, 0, 0, seq, seq_len);
    }
    qsort(seq, *seq_len, sizeof(seq_t), cmp_seq_t);
    int newlen = 0;
    for (int i = 0; i < *seq_len; ++i) {
        if (i == 0 || seq[i] != seq[i - 1])
            seq[newlen++] = seq[i];
    }
    *seq_len = newlen;
}

seq_t a(idx_t n)
{
    static seq_t seq[300000];
    static int seq_len = 0;
    if (seq_len == 0) {
        generate_all(seq, &seq_len);
    }
    if (n == 0 || n > (idx_t)seq_len) return 0;
    return seq[(int)(n - 1)];
}
